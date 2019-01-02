# JustEngine
 A 3D renderer using CPU Instructions and SDL for display and keyboard input handling
 
 To run the program just download and unzip the JustEngine.zip file in this project


### Games:
Currently the only game this supports is a simple game of Pong, but the framework should be setup that any game can be made here (with a couple of limitations at the moment).


### Why are the models so simple?
Since I have the engine set up to take models in through triangle strip format and all the objects were made manually on paper I tried to keep the objects I made simple. The engine can take much more complicated models in, it just requires the vertices to be in triangle strip format as well as an array of texture coordinates to match up with each vertex.


### Does JustEngine support hierarchical models?
At the moment this engine doesn't, but the implementation should be simple as adding another structure on top of the Model class with it's own Translate Rotate and Scale matrices. Only reason this hasn't been put in is I didn't want to focus too much of my efforts on animation when I still was working on getting a basic game put in. I may add this in the future though.


### Why not use ___ API?
I started this project to try and get a lowest level possible understanding for how 3D rendering works and if I take even a little bit of that off my hands then that's just another thing for me to not understand when it comes time to debug with an API. Also, I think that if I can understand what's happening at a base line it'll be much easier to go from one API to another without holding onto each one's specific implementation. In the future though I will be focused on learning DX12 (personal project) and OpenGL (class) so keep an eye on my GitHub for those in the future.
