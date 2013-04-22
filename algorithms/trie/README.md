for info on Tries, look at http://en.wikipedia.org/wiki/Trie

I implemented a basic trie in C. The trie supports only two operations thus far, add and get. Both of these operations are O(l) where l is the length of the key string. Note that I lazily implemented the "dictionary" as simply a pointer array of length 256, therefore able to be uniquely indexed by any ASCII char. A more sophisticated implementation might use a variable-sized hash table. The advantages to what I did are simplicity and speed; the disadvantage is that it's not space efficient: it requires 256 * sizeof(void *), or one or two kilobytes, per node. Of course the number of nodes grows probably logarithmically with the number of names in the trie, so it's not so bad.

For the D implementation, things were much smoother - mostly. I wanted to templatize the D trie so that it could accept any data type. Of course, it still uses pointers (it's easier to be able to return a null reference than having to throw exceptions if a string isn't found, or return a status code and copy into a pointer, or other strategies). The issue then is that for some odd reason I can't fathom, D doesn't allow strings to be allocated with new. This is an annoyance and means that other strategies, like converting a string into a char[] or something else, may have to be used. Anyway, it's progress I guess.