Arduino One Tact Library

This library allows you to use a single push button to be
able to invoke up to 6 different functions!

The button gestures include:

    * No press
    * Single press short (single click)            ---> call func1
    * Single press long  (single click and hold)   ---> call func2
    * Double press short (double click)            ---> call func3
    * Double press long  (double click and hold)   ---> call func4
    * Triple press short (triple click)            ---> call func5
    * Triple press long  (triple click and hold)   ---> call func6

Easy to use. Includes two sample sketches. The first sketch shows how to use the basic functions in a polled manner. The second sketch shows how to register callback functions that will be autmatically called for up to 6 different gestures!
