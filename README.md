# WindowResizingFlickeringDemo
Demonstrate the flickering issue when a window is being resized

Suppose we place two child windows side by side on the main window.
When users resize the main window, the left side child window resizes in proportion to the main window, the rige side child window remains the same size but stick to the right edge of the main window.

We can see that there is no flickering when user resize the window:

![alt tag](ManualResizeWindowNoFlickering.gif?raw=true)

Now we want to allow users to shrink the main window to only show the right side child winow, a simple SetWindowPos causes noticeable flickering.

![alt tag](ResizeWindowLeftEdgeFlickering.gif?raw=true)

Here's the keyframes showing what happens behind the scene:

![alt tag](ResizeWindowLeftEdgeFlickeringStep.gif?raw=true)

However, if the scenario is exactly the opposite, resizing the right edge of the main window to shrink it to the left side does not produce noticeable flickering.


![alt tag](ResizeWindowRightEdgeNoFlickering.gif?raw=true)
