#version 330

// Take in our texture coordinate from our vertex shader
in vec2 texCoords;
// We always define a fragment color that we output.
out vec4 fragColor;

<<<<<<< HEAD
// TODO:  Add a sampler to retrieve our color data from!
uniform sampler2D textureSampler;
=======
uniform sampler2D tex;
>>>>>>> 2c934973a1ad905012aefd5903a948033482eb5c

void main() {
  // Set our output fragment color to whatever we pull from our input texture (Note, change 'tex' to whatever the sampler is named)
  fragColor = texture(textureSampler, texCoords);
}