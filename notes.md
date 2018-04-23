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
