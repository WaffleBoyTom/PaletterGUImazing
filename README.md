# PaletterGUImazing
Palettize your favorite image and apply that palette to another image, or export that palette to use it for something else ! All of that inside a GUI !!!
Shabu Dabu : Paletter GUImazing is the work of Ethan Da Broski and Tiny Tom

cmake --build . 
in build/Deslktop

# TODO
- CUDA:
  - Use to run K-Means for palette generation
  - Use to apply palette to second image
- Add Modes to apply:
  - Added dropdown but does nothing right now.
  - Applying should work on CPU even when we have CUDA and right now, no worko whatsoever...
- color sorting options
- Export:
  - Json ?
  - Export the image with the palette applied
- put quantizer on a worker thread :)

So I think the idea behind SickViewer is that I don't like the palettesource argument to ImageViewer 
so SickViewer would be the base class and then we have two children classes for the two different viewers
I just don't want a ton of bramching logicx in image viewer but maybe three classes is overkill ? mmhhh


IF THIS DOES NOT COMPILE TOM YOU IDIOT
OPEN QTCREATOR AND LOAD THE PROJECT BY CLICKING THE CMAKELIST FILE
QTCREATOR WILL JUST COMPILE THAT SHIT FOR YOU
WE HATE GUIS BUT DAMN THIS SHIT DID THE HOLD ME NOW
CUZ I WAS 6 FEET FROM THE EDGE AND I WAS THINKING
MAYBE SIX FEET AINT SO FAR DOWN
...
