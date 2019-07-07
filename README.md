# DiceWareCLI
A Command-line tool for generating DiceWare passwords
  
DiceWare is a password creation scheme created by Arnold G. Reinhold which builds passwords out of random words. The focal
point of DiceWare is that the random words do not come off the top of your head, and that they instead come from a pre-defined dictionary. This may seem counter-intuitive, as using a predefined dictionary should in theory make passwords easier to crack. However, in practice people cannot come up with truly random words, and as a result the words that they choose out of their mind's dictionary are heavily influenced by a variety of factors. DiceWare is meant to be used with a dice alongside a list of words corresponding to each combination of rolls. Using a command-line tool like this is a lot more convenient.  
Word lists were gotten from:
 - https://github.com/first20hours/google-10000-english/blob/master/20k.txt
 - https://github.com/first20hours/google-10000-english/blob/master/google-10000-english-usa-no-swears-medium.txt  
 
They were then filtered to remove words less than 5 characters and greater than 8 characters, and then filtered again to remove words in this list of profanity
 - https://github.com/LDNOOBW/List-of-Dirty-Naughty-Obscene-and-Otherwise-Bad-Words/  
 
*Usage*  
```c
DiceWare <Words per password entry> [<Number of passwords> = 1] [<Name of Transformation File>]
```

Since the code is written in C and not a JVM language, it will most likely be necessary to compile it manually. However, if by some miracle you are running 64-bit Ubuntu, you may be able to run the pre-compiled binary.
If you are on Windows, putting your compiled code into your PATH will allow you to call it from any directory.
To generate passwords, run the compiled program with the specified parameters. Parameters encompassed in square brackets are optional, and those with default values are specified after '='
