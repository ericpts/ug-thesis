# Optimizarea fisierelor Java

# Abstract

In acest paper voi descrie incercarea mea de a crea un optimizator de spatiu
pentru fisierele .class ale limbajului Java.

Acest optimizator se bazeaza pe analiza statica a fisierelor pentru eliminarea
metodelor nefolosite din cadrul fisierelor.

Voi analiza structura fisierelor, voi explica modul de analiza alor si voi
expune modul de eliminare a metodelor.

# Introducere

## Java

Java este un limbaj de programare orientat pe obiecte.
Acesta a fost dezvoltat de catre Sun Microsystems (
acum Oracle), iar prima versiune a aparut in anul 1995.

Jaza s-a bazat pe sintaxa limbajului C, si a introdus notiunea de "scrie o data, ruleaza peste tot" (eng. "write once, run everywhere").
Spre deosebire de C si de C++, care trebuiesc compilate pentru fiecare platforma tinta, Java a avut avantajul ca trebuie compilat o singura data, si va merge garantat pe toate platformele suportate de limbaj.

### Java Bytecode

Solutia limbajului Java pentru a fi independent de platforma este de transforma
codul intr-o reprezentare intermediara, in loc de direct in cod binary pentru o anumita arhitectura .

Compilatorul Java (`javac`), transforma codul Java intr-un limbaj intermediar, numit Java Bytecode.

Acest limbaj este un limbaj low-level, destinat in mod exclusiv procesarii de catre masini, spre deosebire de codul Java, care este destinat oamenilor.

Dupa ce compilatorul a procesat codul Java, provenit din fisere .java in format text,
acesta salveaza rezultatul in fisiere de tip clasa (.class) in format binar.

### Masina Virtuala Java (JVM)

Odata generate fisierele binare, acestea sunt executate pe o masina virtuala specifica limbajului Java - numita JVM sau `The JVM` (eng. Java Virtual Machine).

Aceasta masina virtuala are rolul de a citi fisierele de clasa binare si de a le interpreta.

Masina virtuala este implementata ca o "masina cu stiva" (eng. stack machine),
unde toate instructiunile limbajului bytecode interactioneaza cu datele de pe o stiva controlata de aplicatie.

Masina virtuala insusi este implementata in C/C++, si este compilata in cod binar direct, dependent de arhitectura.
Dezvoltatorii limbajului Java sunt responsabili pentru corectitudinea si siguranta
masinii virtuale, in timp ce dezvoltatorii de aplicatii Java au garantia ca daca
codul lor Java este corect, atunci acesta va rula la fel, deterministic, pe orice platforma.

In acest regard, limbajul Java poate fi vazut ca un limbaj interpretat.
Comparand cu alte limbaje populare interpretate, ca de exemplu Python, Ruby, sau Perl, ne-am astepta ca si Java sa fie la fel de incet ca acestea [1].
Totusi, Java obtine performante mult mai bune decat aceastea. Acest fapt se datoreaza
compilarii tocmai-la-timp (eng. just-in-time), in care atunci cand interpretorul
observa o secventa de cod care este interpretata repetitiv de foarte multe ori,
va genera direct cod binary pentru aceasta.

# Fisierele de clasa

Fisierele de clasa Java sunt formate din 10 sectiuni[2]:

1. Constanta magica.
2. Versiunea fisierului.
3. Constantele clasei.
4. Permisiunile de acces.
5. Numele clasei din fisier.
6. Numele superclasei.
7. Interfetele pe care clasa le implementeaza.
8. Campurile clasei.
9. Metodele clasei.
10. Atribute ale clasei.

In continuare voi da o scurta descriere a formatului sectiunilor.

## Sectiunile fiserelor clasa


### Magic

Toate fiserele clasa trebuiesc sa inceapa cu un numar denumit constanta magica.
Acesta este folosit pentru a identifica in mod unic ca acestea sunt intra-devar fisiere clasa.
Numarul magic are o valoare memorabila: reprezentarea hexadecimala este `0xCAFEBABE`,

### Versiunea

Versiunea unui fisier clasa este data de doua valori, versiunea majora `M` si versiunea minora `m`.
Versiunea clasei este atunci reprezentata ca `M.m`. (e.g., `45.1`).
Aceasta este folosita pentru a mentine compatibilitatea in cazul modificarilor masinii virtuale care interpreteaza clasa sau ale compilatorului care o genereaza.

### Constantele clasei

Tabela de constante este locul unde sunt stocate valorile literale constante ale clasei:
	* Numere intregi.
	* Numere cu virgula mobula.
	* Siruri de caractere, care pot reprezenta la randul lor:
		* Nume de clase.
		* Nume de metode.
		* Tipuri ale metodelor.
	* Informatii compuse din datele anterioare:
		* Referinta la o metoda a unei clase.
		* Referinta la o constanta a unei clase.

Toate celelalte tipuri de date compuse, cum ar fi metodele sau campurile, vor contine indecsi in tabela de constante.

### Permisiunile de acces

Aceste permisiuni constau intr-o masca de bitsi, care reprezeinta operatiile permise pe aceasta clasa:

	* daca clasa este publica, si poate fi accesta din afara pachetului acesteia.
	* daca clasa este finala, si daca poate fi extinsa.
	* daca invocarea metodelor din superclasa sa fie tratata special.
	* daca este de fapt o interfata, si nu o clasa.
	* daca este o clasa abstracta si nu poate fi instatiata.

### Clasa curenta

Reprezinta un indice in tabela de constante, unde sunt stocate informatii despre clasa curenta.

### Clasa super

Reprezinta un indice in tabela de consatante, cu informatii despre clasa din care a mostenit clasa curenta.
Daca este 0, inseamna ca clasa curenta nu mosteneste nimic: singura clasa fara o superclasa este clasa `Object`.


    u2             this_class;
    u2             super_class;
    u2             interfaces_count;
    u2             interfaces[interfaces_count];
    u2             fields_count;
    field_info     fields[fields_count];
    u2             methods_count;
    method_info    methods[methods_count];
    u2             attributes_count;
    attribute_info attributes[attributes_count];


Asadar, putem rezuma tipuri de date intalnite in fisierele de clasa in urmatoarele tabele:

### Tipurile de baza

In fisiere clasa exista trei tipuri de baza, toate bazate pe intregi.
In caz ca un intreg are mai multi octeti, acestia au ordinea de `big-endian`: cel mai semnificativ octet va fi mereu primul in memorie.


| Nume | Semantica                       | Echivalentul in C               |
|:----:|:-------------------------------:|:-------------------------------:|
| `u1` | intreg pe un octet, fara semn   | `unsigned char` sau `uint8_t`   |
| `u2` | intreg pe doi octeti, fara semn | `unsigned short` sau `uint16_t` |
| `u4` | intreg pe un octet, fara semn   | `unsigned int` sau `uint32_t`   |


### Tipuri de date compuse


#### cp\_info
#### field\_info
#### method\_info
#### attribute\_info

In C/C++, acesta ar arata astfel:
```cpp
struct ClassFile {
    u4             magic;
    u2             minor_version;
    u2             major_version;
    u2             constant_pool_count;
    cp_info        constant_pool[constant_pool_count-1];
    u2             access_flags;
    u2             this_class;
    u2             super_class;
    u2             interfaces_count;
    u2             interfaces[interfaces_count];
    u2             fields_count;
    field_info     fields[fields_count];
    u2             methods_count;
    method_info    methods[methods_count];
    u2             attributes_count;
    attribute_info attributes[attributes_count];
}
```


[1] https://github.com/trizen/language-benchmarks

[2] https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html
