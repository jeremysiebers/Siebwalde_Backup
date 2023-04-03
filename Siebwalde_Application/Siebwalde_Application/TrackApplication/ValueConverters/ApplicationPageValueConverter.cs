﻿using System;
using System.Diagnostics;
using System.Globalization;

// https://youtu.be/JU9Ai0hpBUI?t=660

namespace Siebwalde_Application
{
    /// <summary>
    /// Converts the <see cref="ApplicationPage"/> to an actual view/page
    /// </summary>
    public class ApplicationPageValueConverter : BaseValueConverters<ApplicationPageValueConverter>
    {
        public override object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            switch ((ApplicationPage)value)
            {
                case ApplicationPage.TrackControlView:
                    return new TrackControlView();

                case ApplicationPage.TrackAmplifierItemView:
                    return new TrackAmplifierItemView();

                case ApplicationPage.TrackAmplifierManualControlView:
                    return new TrackAmplifierManualControlView();

                default:
                    Debugger.Break();
                    return null;
            }
        }

        public override object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
