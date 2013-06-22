# The "Box Dilemma" and Implicit Unboxing

The "*Box Dilemma*" is the name I've given to the situation where modifying an interface to provide additional information requires making a non-backwards-compatible change.

"*Implicit Unboxing*" is a possible solution to this problem.  The compiler or interpreter would let you work with an object (the "box") containing some value you are interested in wrapped up with some other stuff you want to ignore, and write code as if the box and the other stuff didn't exist.


## When a Box is a Problem

Imagine you walk into a jewelry store that offers watch polishing services.  The sign says "give us a watch, and we will give you back a polished watch."  Fantastic.  You hand the man behind the counter your fancy watch, in its original box.

**Jeweler**: *What's this?*

**You**: *A watch.*

**Jeweler**: *This isn't a watch.  It's a box.*

**You**: *Uh…the watch is *in* the box.*

**Jeweler**: *I told you I polish watches, not boxes.*

**You**: *Are you kidding me?*

You expect human beings to have some common sense when dealing with boxes.  *Just open the box!*  But, for some reason, we don't have the same expectations for software.  If you are using some sort of interface -- a library, a web service, a function, etc. -- and it expects a watch, and you pass it a box with a watch inside of it, it won't work.  For example:

	Watch polish(Watch watch)
	jeweler.polish(new Box(new Watch))

If it's a strongly typed language, the compiler will respond like that jeweler did, with a type error, something like:

	'polish' expects an argument of type Watch.  But you passed it an argument of type Box[Watch] (you moron).
	
If the language is not strongly typed, it will act even less intelligently, and throw a disasterous runtime error only after it has ripped your cardboard box to shreds with its diamond-embedded industrial polishing wheel.

	'hardness' is undefined at polish.script line 230

The same problem happens if the Jeweler return the watch in a box containing, say, the receipt, but your code wasn't written to expect a box.


## Interface Breaking Changes

As a less allegorical example, suppose I have a function that uses some statistics to guess the language of a natural language string.  The fairly unsophisticated function just returns a string with the name of the language, which is all I need.  It's not always 100% confident of its guess, so it also returns a number representing a confidence %.  The interface looks like this (in no particular programming language):

#### Listing 1

	interface LanguageGuess
		language: String
		confidence: Float
	
	guessLanguage: String => LanguageGuess
	
But I am not interested in the `confidence` field now.  I wish the interface wasn't so complicated and I could just treat the return value as a string, and write code like this:

#### Listing 2

	language = guessLanguage("oh la la")
	println language + ": " + if(language == "French")
		"that's what I thought"
	else 
		"what the?!?!"	

You may be thinking "what an incredibly lazy programmer!"  Is it really so much work to write `language.confidence` instead of `language`?

Well, no.  My motivation is not saving keystrokes.  Rather, it's that I have some history with this function.  It actually used to return just a string.  It was very simple and straightforward, until I tried this:

#### Listing 3

	language = GuessLanguage("no")
	println language + ": " + if(language == "English")
		"that's what I thought"
	else 
		"what the?!?!"
	
Result

	Spanish: what the?!?!?

Crap!  I realized that some words like "no" exist in many languages, and that my previous thinking about this language identification problem was over-simple.  I needed to work the ideas of ambiguity and/or confidence into my function.

So I changed the interface to return an object, containing both a language and a confidence %.  So at least I would know when I wasn't sure of my guess:

	guess = guessLanguage("no")
	if(guess.confidence > .95)
		"The language is " + guess.language
	else
		"I don't know"
	
The problem is, I have now broken all the code I wrote that uses the old interface.  Thus my desire for the code in Listing 2 to work without any change.

Changing little bits of code like this when you change an interface is no big deal if you are a maveric solo programmer, but if you are part of a team or an open source project, and a lot of people have written code using the original interface, it can be a problem.  


### The "Core Value" and the "Box"

With many data structures, it's easy to identify the *core* value.  It's the payload, the meat, the fancy watch, the T type parameter, the thing that the "container" exists to contain.  

Changing an interface that used to return a value of type T, to return some sort of object *containing* a value of type T, is one of the most common and expensive changes coders make, one I beleive accounts for measureless software development costs annually.

But this sort of change is a necessary part of the natural evolution of code.  First of all, it's easier to start thinking about your program logic in terms of these core functions and types, and initially ignore ancilliary stuff like logging, error handling, etc.  Second, you like all of us have a limited IQ and simply won't think of all the aspects that should be worked into your design up front (and gosh darnit I'm sick of languages making my feel like I should).  And finally, requirements and designs change!


### A Box Monad?

Many of the millions of attempts to explain what a Monad is use a Box as an analogy.  Now, Monads go beyond simply a value contained in a box.  They can deal with future values, or multiple values, or possible or probable values, and so on.  

Rather, the "Box Dilemma" problem is just one problem that Monads may help solve.

One of the things that gets functional programmers excited about Monads, is that they kind of promise to let you write code that ignores it's environment (in this case, the box).   But you have to write your code just a little differently so the compiler or interpreter knows to work the Monad magic.  For example if I made the type of objects returned by guessLanguage into Monad instances, I could use 'do' syntax (a la Haskell) or for comprehensions (a la Scala), like this:

#### Listing 4 (Scala-like for comprehension)

	for
		language <- guessLanguage("oh la la")
		yield println language + ": " + if(language == "French")
			"that's what I thought"
		else 
			"what the?!?!"
	

This is, uhh, great.  It makes my code more complicted, and it hasn't helped me achieve my goal of not changing Listing 2.

On the bright side, *this* code is now more robust, because now its possible to add additional aspects to the objects returned by guessLanguage, such as opssible errors, or multiple values, or I could even make it run asynchronously, without changing the code in Listing 4 (although combining all these aspects into one object would be challenging.)  Now that is kind of neat, and part of why Monads are all the rage right now.  


### Implicit Unboxing

But, I'm not satisfied.  I don't want to change Listing 2 one bit.  My core logic was expressed perfectly clearly and succinctly in listing 2.  I want to **completely** de-couple my core logic from ancilliary aspects of my program like confidence and possible error and logging. 

Why can't the compiler look at my code in listing 1, and automatically just do the right thing?  It could automatically convert listing 1 to listing 2 (if we wanted to use a Monad for the implementation).  Or it could simply rewrite the code to extract the core value explicitly.

One reason I might not want this, is that in some cases I *do* want access to the other stuff in the box.  So I'd need a way to indicate these cases to the compiler.  One possibly  solution may be to use type annotations (assuming a typed language) to indicate either the simpler type or the whole box.   If I want to access the whole box, my code might look like this:

#### Listing 5

	/* Treat the return value as a language estimate */
	estimate: LanguageEstimate = guessLanguage("quiero taco bell")

	println "The language is " + estimate.language + " with confidence " + estimate.string 

Listing 2 would still work as it is.


### Creating Boxes

Suppose in our language, we could create objects using syntax like this:

	(-core "French", -confidence 30)

And to save some keystrokes, lets say that we can indicate the "core" value just by listing it first.

	("French", -confidence 30)

The compiler would ignore everything but the core value by default

	language = ("French", -confidence 30)
	println language
	
Output

	French
	
As it would if I use a type annotation compatible with the core value

	language: String = ("French", -confidence 30)
	println language

Output
	
	French

But if I use a type annotation compatible with the box, it would give me access to the box and all the extra stuff in it.

	estimate: LanguageEstimate = ("French", -confidence 30)
	println "Language is " + estimate.language + " with confidence " + estimate.confidence

Structural typing could come in handy here.  Suppose the following syntax lets me define a type that has a `core` value of type String and a `confidence` of type Float.
	
	LanguageEstimate = type(String, -confidence: Float)		
And a syntax for deconstructing assignments could also come in handy

	(language, -confidence)	= ("French", -confidence 30)
	println "Language is " + language + " with confidence " + confidence

Now, if I originally wrote my function to only return a string:

	estimateLanguage = string ->
		(language: String, confidence: Float) = /* do some analysis */
		/* return language */
		language 	
			
I could modify it return confidence as well without breaking any existing code just by returning an object with a core value.

	LanguageEstimate = type(String, -confidence: Float)	estimateLanguage = s ->
		(language: String, confidence: Float) = /* do some analysis on s */
		/* return language and confidence*/
		(language, -confidence confidence)

## Conclusion

Since clearly defining the "Box Dilemma" in my own mind, and understanding what seems like a plausible solution, I have seen countless instances of where this would be immensely useful.  

For example, it would provide an interesting way of supporting macros in a language.  In an expression such as f(g(x)), the value of g(x) would normally be passed to f.  What if, instead, f received a box that contained the value of g(x) as the core (evaluated lazily), but also contained a data structure representing the parsed expression g(x).  If you are writing a regular function, you would ignore the box.  If you are writing a macro, you could access the box and manipulate the expression itself.

It can allow you to sneak in "extra" information in all sorts of places where normally it wouldn't fit.  For example, suppose I have a println function defined for strings, which are just lists of characters.  And suppose I have defined constants for text formatting such as color and style.  I could create strings that have format annotations, and pass those strings to functions that don't know anything about color annotations, without breaking anything.

	colorfulString = [
			('a', -color red)
			('b')
			('c', -color blue, -bold)
		]
		
	println colorfulString

#### Output

	abc	

But other functions that know about text formatting could deal with them.

A language that supported implicit unboxing would open up all sorts of possibilities, where new information could be added to interfaces were previously we wouldn't have dreamed of it because of how much complexity it would add to the interface, and how much existing code it would break.  It allows functions to return error, debug, type and contract, annotations, and all sorts of other information that can be ignored by code that doesn't need it.  But most importantly, as you change your interface due to the natural evolution of your understanding of the problem and your requirements, you are much less likely to break existing code.





