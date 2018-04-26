## Class file format
Formatul class file:

1. 0xCAFEBABE la inceputul fisierului.
2. Un numar pe 2 bytes: versiunea minora a formatului
3. Un numar pe 2 bytes: versiunea majora a formatului:
  * Java SE 10 - 54
  * Java SE 9 - 53
  * Java SE 8 - 52
  * Java SE 7 - 51
  * Java SE 6.0 - 50
  * Java SE 5.0 - 49
  * JDK 1.4 = 48
  * JDK 1.3 = 47
  * JDK 1.2 = 46
  * JDK 1.1 = 45
4. Un numar pe 2 bytes: numarul de entry-uri in pool-ul de constante: ncpents
5. Pool-ul de constante, de dimensiunie variabila cpsize. Acesta contine numere, string-uri si referinte la clase si metode.
Este indexat de la 1 si contine (ncpents - 1) elemente.


## Constant Pool

Tabela de constante are dimensiune ncpool - 1 din motive istorice.
Fiecare constanta are un anumit tip, dintre 14 posibile, seminificat printr-un tag de 1 byte la inceputul blocului.
Apoi, urmeaza fie direct valoare constantei (in cazul majoritatii dintre ele), fie un u2 care reprezeinta lungimea string-ului, in cazul ca tag-ul este 1: acest tag reprezinta un sir de caractere pseudo-UTF8.

| Tag byte | Additional bytes | Description of constant |
| -------- | ---------------- | ----------------------- |
| 1	       | 2+x bytes        | (variable)	UTF-8 (Unicode) string: a character string prefixed by a 16-bit number (type u2) indicating the number of bytes in the encoded string which immediately follows (which may be different than the number of characters). Note that the encoding used is not actually UTF-8, but involves a slight modification of the Unicode standard encoding form. |
| 3	       | 4 bytes          | Integer: a signed 32-bit two's complement number in big-endian format
| 4	       | 4 bytes          | Float: a 32-bit single-precision IEEE 754 floating-point number
| 5	       | 8 bytes          | Long: a signed 64-bit two's complement number in big-endian format (takes two slots in the constant pool table)
| 6	       | 8 bytes          | Double: a 64-bit double-precision IEEE 754 floating-point number (takes two slots in the constant pool table)
| 7	       | 2 bytes          | Class reference: an index within the constant pool to a UTF-8 string containing the fully qualified class name (in internal format) (big-endian)
| 8	       | 2 bytes          | String reference: an index within the constant pool to a UTF-8 string (big-endian too)
| 9	       | 4 bytes          | Field reference: two indexes within the constant pool, the first pointing to a Class reference, the second to a Name and Type descriptor. (big-endian)
| 10       | 4 bytes          | Method reference: two indexes within the constant pool, the first pointing to a Class reference, the second to a Name and Type descriptor. (big-endian)
| 11       | 4 bytes	      | Interface method reference: two indexes within the constant pool, the first pointing to a Class reference, the second to a Name and Type descriptor. (big-endian)
| 12       | 4 bytes          | Name and type descriptor: two indexes to UTF-8 strings within the constant pool, the first representing a name (identifier) and the second a specially encoded type descriptor.
| 15       | 3 bytes          | Method handle: this structure is used to represent a method handle and consists of one byte of type descriptor, followed by an index within the constant pool.[5]
| 16       | 2 bytes          | Method type: this structure is used to represent a method type, and consists of an index within the constant pool.[5]
| 18       | 4 bytes          | InvokeDynamic: this is used by an invokedynamic instruction to specify a bootstrap method, the dynamic invocation name, the argument and return types of the call, and optionally, a sequence of additional constants called static arguments to the bootstrap method.[5]

