A Mandelbrot viewer with zoom and pan for the TTGO ESP32 T-Display 1.14.

Features

Zoom in and out of the Mandelbrot set

Pan across the fractal in all directions

Interactive menu to control zoom and pan


Setup

Ensure the TFT_eSPI library is properly configured for the TTGO T-Display:

  Navigate to the TFT_eSPI library folder (Arduino/libraries/TFT_eSPI/User_Setup.h).
  
  Edit User_Setup.h to match the TTGO T-Display 1.14 configuration.

Copy the Mandelbrot viewer code into the Arduino IDE.
Select TTGO T1 as the board in the Arduino IDE.
Connect your TTGO T-Display 1.14 to your computer via USB.
Upload the sketch.

Usage

Button 1: Cycle through menu options (Zoom In, Zoom Out, Reset View, Pan Left, Pan Right, Pan Up, Pan Down).
Button 2: Execute the selected option.
