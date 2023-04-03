/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2009, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/**
 * \file
 *
 * Implementation of touchscreen driver device irrelevance code.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>

#include "tsd.h"
#include <board.h>
#include <pio/pio.h>
#include <lcd/lcdd.h>
#include <lcd/draw.h>
#include <lcd/font.h>
#include <lcd/color.h>
#include <utility/assert.h>
#include "tsd/tsd_ads7843.h"

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Size in pixels of calibration points. */
#define POINTS_SIZE         4
/** Maximum difference in pixels between the test point and the measured point. */
#define POINTS_MAX_ERROR    8

/** Delay at the end of calibartion for result display */
#define DELAY_RESULT_DISPLAY 4000000

/*----------------------------------------------------------------------------
 *        Types
 *----------------------------------------------------------------------------*/

/** Point used during the touchscreen calibration process. */
typedef struct _CalibrationPoint {

    /** Coordinate of point along the X-axis of the screen. */
    uint32_t x;
    /** Coordinate of point along the Y-axis of the screen. */
    uint32_t y;
    /** Calibration data of point. */
    uint32_t data[2];

} CalibrationPoint;

/*----------------------------------------------------------------------------
 *        Variables
 *----------------------------------------------------------------------------*/

/** indicates if the touch screen has been calibrated. */
/** If not, Callback functions are not called. */
static volatile uint8_t bCalibrationOk = 1 ; //0;
/** Slope for interpoling touchscreen measurements along the X-axis. */
static int32_t xSlope;
/** Slope for interpoling touchscreen measurements along the Y-axis. */
static int32_t ySlope;

/** Calibration points. */
static CalibrationPoint calibrationPoints[] = {

    /* Top-left corner calibration point */
    {
        BOARD_LCD_WIDTH / 10,
        BOARD_LCD_HEIGHT / 10,
        {0, 0}
    },
    /* Top-right corner calibration point */
    {
        BOARD_LCD_WIDTH - BOARD_LCD_WIDTH / 10,
        BOARD_LCD_HEIGHT / 10,
        {0, 0}
    },
    /* Bottom-right corner calibration point */
    {
        BOARD_LCD_WIDTH - BOARD_LCD_WIDTH / 10,
        BOARD_LCD_HEIGHT - BOARD_LCD_HEIGHT / 10,
        {0, 0}
    },
    /* Bottom-left corner calibration point */
    {
        BOARD_LCD_WIDTH / 10,
        BOARD_LCD_HEIGHT - BOARD_LCD_HEIGHT / 10,
        {0, 0}
    }
};

/** Test point */
static const CalibrationPoint testPoint = {
    BOARD_LCD_WIDTH / 2,
    BOARD_LCD_HEIGHT / 2,
    {0, 0}
};

/*----------------------------------------------------------------------------
 *        External functions
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/*
 * \brief Display a calibration point on the given buffer.
 *
 * \param pPoint  Calibration point to display.
 */
static void DrawCalibrationPoint(const CalibrationPoint *pPoint)
{
    LCDD_DrawRectangleWithFill(pPoint->x - POINTS_SIZE / 2,
                               pPoint->y - POINTS_SIZE / 2,
                               POINTS_SIZE,
                               POINTS_SIZE,
                               COLOR_RED);
}

/**
 * \brief Clears a calibration point from the given buffer.
 *
 * \param pPoint  Calibration point to clear.
 */
static void ClearCalibrationPoint(const CalibrationPoint *pPoint)
{
    LCDD_DrawRectangleWithFill(pPoint->x - POINTS_SIZE / 2,
                               pPoint->y - POINTS_SIZE / 2,
                               POINTS_SIZE,
                               POINTS_SIZE,
                               COLOR_WHITE);
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \breif Indicates if the calibration of the touch screen is Ok.
 *
 * \return 1 calibration Ok, 0 if not.
 */
uint8_t TSDCom_IsCalibrationOk(void)
{
    if (bCalibrationOk == 1) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * \brief Interpolates the provided raw measurements using the previously calculated
 * slope. The resulting x and y coordinates are stored in an array.
 *
 * \param pData  Raw measurement data, as returned by TSD_GetRawMeasurement().
 * \param pPoint  Array in which x and y will be stored.
 */
void TSDCom_InterpolateMeasurement(const uint32_t *pData, uint32_t *pPoint)
{
    pPoint[0] = calibrationPoints[0].x
                - (((int32_t) calibrationPoints[0].data[0] - (int32_t) pData[0]) * 1024)
                / xSlope;

    pPoint[1] = calibrationPoints[0].y
                - (((int32_t) calibrationPoints[0].data[1] - (int32_t) pData[1]) * 1024)
                / ySlope;

    if(pPoint[0] & 0x80000000) /* Is pPoint[0] negative ? */
    {
        pPoint[0] = 0;
    }

    if(pPoint[0] > BOARD_LCD_WIDTH) /* Is pPoint[0] bigger than the LCD width ? */
    {
        pPoint[0] = BOARD_LCD_WIDTH;
    }

    if(pPoint[1] & 0x80000000) /* Is pPoint[1] negative ? */
    {
        pPoint[1] = 0;
    }

    if(pPoint[1] > BOARD_LCD_HEIGHT) /* Is pPoint[1] bigger than the LCD width ? */
    {
        pPoint[1] = BOARD_LCD_HEIGHT;
    }
}

/**
 * \brief Performs the calibration process using the provided buffer to display
 * information.
 *
 * \return True if calibration was successful; otherwise false.
 */
uint8_t TSDCom_Calibrate(void)
{
    volatile uint32_t i; /* to keep the tempo with gcc code optimisation */
    int32_t slope1, slope2;
    CalibrationPoint measuredPoint;
    uint8_t xOk, yOk;
    int32_t xDiff, yDiff;

    /* Calibration setup */
    LCDD_Fill(COLOR_WHITE);
    LCDD_DrawString(30, 50, (uint8_t *)"LCD calibration", COLOR_BLACK);
    LCDD_DrawString(1, 140, (uint8_t *)"Touch the dots to\ncalibrate the screen", COLOR_DARKBLUE);

    /* Calibration points */
    for (i=0; i < 4; i++) {

        DrawCalibrationPoint(&calibrationPoints[i]);

        /* Wait for touch & end of conversion */
        TSD_WaitPenPressed();
        TSD_GetRawMeasurement(calibrationPoints[i].data);
        ClearCalibrationPoint(&calibrationPoints[i]);

        /* Wait for contact loss */
        TSD_WaitPenReleased();
    }

    /**
     * Calculate slopes using the calibration data
     * Theory behind those calculations:
     *   - We suppose the touchscreen measurements are linear, so the following equations are true (simple
     *     linear regression) for any two 'a' and 'b' points of the screen:
     *       dx = (a.data[0] - b.data[0]) / (a.x - b.x)
     *       dy = (a.data[1] - b.data[1]) / (a.y - b.y)
     *
     *   - We calculate dx and dy (called xslope and yslope here) using the calibration points.
     *
     *   - We can then use dx and dy to infer the position of a point 'p' given the measurements performed
     *     by the touchscreen ('c' is any of the calibration points):
     *       dx = (p.data[0] - c.data[0]) / (p.x - c.x)
     *       dy = (p.data[1] - c.data[1]) / (p.y - c.y)
     *     Thus:
     *       p.x = c.x - (p.data[0] - c.data[0]) / dx
     *       p.y = c.y - (p.data[1] - c.data[1]) / dy
     *
     *   - Since there are four calibration points, dx and dy can be calculated twice, so we average
     *     the two values.
     */
    slope1 = ((int32_t) calibrationPoints[0].data[0]) - ((int32_t) calibrationPoints[1].data[0]);
    slope1 *= 1024;
    slope1 /= ((int32_t) calibrationPoints[0].x) - ((int32_t) calibrationPoints[1].x);
    slope2 = ((int32_t) calibrationPoints[2].data[0]) - ((int32_t) calibrationPoints[3].data[0]);
    slope2 *= 1024;
    slope2 /= ((int32_t) calibrationPoints[2].x) - ((int32_t) calibrationPoints[3].x);
    xSlope = (slope1 + slope2) / 2;

    slope1 = ((int32_t) calibrationPoints[0].data[1]) - ((int32_t) calibrationPoints[2].data[1]);
    slope1 *= 1024;
    slope1 /= ((int32_t) calibrationPoints[0].y) - ((int32_t) calibrationPoints[2].y);
    slope2 = ((int32_t) calibrationPoints[1].data[1]) - ((int32_t) calibrationPoints[3].data[1]);
    slope2 *= 1024;
    slope2 /= ((int32_t) calibrationPoints[1].y) - ((int32_t) calibrationPoints[3].y);
    ySlope = (slope1 + slope2) / 2;

    /* Test point */
    LCDD_Fill(0xFFFFFF);
    LCDD_DrawString(30, 50, (uint8_t *)"LCD calibration", COLOR_BLACK);
    LCDD_DrawString(1, 100, (uint8_t *)" Touch the point to\nvalidate calibration", COLOR_DARKBLUE);
    DrawCalibrationPoint(&testPoint);

    /* Wait for touch & end of conversion */
    TSD_WaitPenPressed();

    TSD_GetRawMeasurement(measuredPoint.data);
    TSDCom_InterpolateMeasurement(measuredPoint.data, (uint32_t *) &measuredPoint);
    DrawCalibrationPoint(&measuredPoint);

    /* Check resulting x and y */
    xDiff = (int32_t) measuredPoint.x - (int32_t) testPoint.x;
    yDiff = (int32_t) measuredPoint.y - (int32_t) testPoint.y;
    xOk = (xDiff >= -POINTS_MAX_ERROR) && (xDiff <= POINTS_MAX_ERROR);
    yOk = (yDiff >= -POINTS_MAX_ERROR) && (yDiff <= POINTS_MAX_ERROR);

    /* Wait for contact loss */
    TSD_WaitPenReleased();

    /* Check calibration result */
    if (xOk && yOk) {

        bCalibrationOk = 1;
        LCDD_Fill(COLOR_WHITE);
        LCDD_DrawString(30, 50, (uint8_t *)"LCD calibration", COLOR_BLACK);
        LCDD_DrawString(80, 140, (uint8_t *)"Success !", COLOR_GREEN);

    }
    else {

        bCalibrationOk = 0;
        LCDD_Fill(COLOR_WHITE);
        LCDD_DrawString(30, 50, (uint8_t *)"LCD calibration", COLOR_BLACK);
        LCDD_DrawString(40, 140, (uint8_t *)"Error too big", COLOR_RED);
    }

    /* Slight delay */
    for (i = 0; i < DELAY_RESULT_DISPLAY; i++);

    return (xOk && yOk);
}

/**
 * \brief Read calibrate data to buffer.
 *
 * \param pBuffer  Data buffer.
 * \param size     Size of data buffer in bytes.
 */
void TSDCom_ReadCalibrateData(void *pBuffer, uint32_t size)
{
    uint8_t *pDest = (uint8_t *)pBuffer;

    SANITY_CHECK((sizeof(bCalibrationOk) + sizeof(xSlope) +
        sizeof(ySlope) + sizeof(calibrationPoints[0].data)) < size);

    memcpy(pDest, (void const *)&bCalibrationOk, sizeof(bCalibrationOk));
    pDest += sizeof(bCalibrationOk);
    memcpy(pDest, &xSlope, sizeof(xSlope));
    pDest += sizeof(xSlope);
    memcpy(pDest, &ySlope, sizeof(ySlope));
    pDest += sizeof(ySlope);
    memcpy(pDest, &calibrationPoints[0].data, sizeof(calibrationPoints[0].data));
    pDest += sizeof(calibrationPoints[0].data);
}

/**
 * \brief Restore calibrate data with buffer data.
 *
 * \param pBuffer  Data buffer.
 * \param size     Size of data buffer in bytes.
 */
void TSDCom_RestoreCalibrateData(void *pBuffer, uint32_t size)
{
    uint8_t *pSrc = (uint8_t *)pBuffer;

    SANITY_CHECK((sizeof(bCalibrationOk) + sizeof(xSlope) +
        sizeof(ySlope) + sizeof(calibrationPoints[0].data)) < size);

    memcpy((void *)&bCalibrationOk, pSrc, sizeof(bCalibrationOk));
    pSrc += sizeof(bCalibrationOk);
    memcpy(&xSlope, pSrc, sizeof(xSlope));
    pSrc += sizeof(xSlope);
    memcpy(&ySlope, pSrc, sizeof(ySlope));
    pSrc += sizeof(ySlope);
    memcpy(&calibrationPoints[0].data, pSrc, sizeof(calibrationPoints[0].data));
    pSrc += sizeof(calibrationPoints[0].data);
}
