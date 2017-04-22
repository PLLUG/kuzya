![](https://github.com/PLLUG/kuzya/blob/development/src/images/Kuzya_about.png)

#How to build Kuzya

Before build Kuzya you need to run Python script `src/generate.py`. This script builds a special file in which the specified version and date of build. To run this script you must have installed (for your system) [**Python**](https://www.python.org/downloads/), or [**Git**](https://git-scm.com/downloads).

###To generate that special file you must follow these steps:
* open a _**command line**_ on your system (if you installed _Python_), or open a _**Git Bash**_ (if you installed _Git_);
* go to the folder `src/` in Kuzya project (`cd kuzya/src`);
* run the `generate.py` with key `-v` and number build _(MAJ.MIN.MIC)_. For example: `python generate.py -v 5.7.99`;

To build Kuzya in to your system first of all there should be installed Qt 5.8, QScintilla 2.4.9 and Microsoft Visual Studio Compiler 2015_64bit (MVSC 2015_64bit).