# Chaos Engine

Chaos Engine is a LED framework built to be used with an Arduino (or equivilent) and a 1D led strip.

It heavily utilizes the FastLED library.

## Usage

### Update Config
Make sure you open up `config.h` and adjust any of the defines to properly reflect your setup.

### Creating Patterns

Make a copy of pattern_template.h and rename it what you'd like your new pattern to be called. Inside of the file, make sure you rename the various parts inside of the file to your pattern name as well. Search and replace `PATTERN_NAME_H` and `PatternName`.

Put all setup code in the constructor.

Any variables that need to be reset at the start of the pattern in `Reset()`

Put your code that will generate one frame of your animation into `Generate(CRGB arr*)`. Most example code will have you adjust the led array but to use this framework, use the arr parameter instead.

## Major TODOs
* Restructure code to be more like a library.
* Create a working makefile and document the environment setup.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)<Paste>
