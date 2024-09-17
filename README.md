# Aplicatie HTTP - Client REST API

Copyright 2023 - 2024 Ionita Alexandru-Andrei 322CA

## Descriere

Aceasta aplicatie reprezinta un client HTTP care permite utilizatorului sa faca
cereri unui server biblioteca. Acesta poate crea, sterge si vizualiza detalii
despre carti. Acest server are un sistem de autentificare si autorizare:

- Autentificare: se face printr-un cookie de sesiune
- Autorizare: se face printr-un token JWT obtinut la intrarea in biblioteca

## Detalii implementare

### Autentificare

Pentru a putea accesa resursele serverului, clientul trebuie sa fie autentificat.
Acest lucru se face printr-un cookie de sesiune care este obtinut la autentificare.

### Autorizare

Pentru a putea accesa resursele serverului, clientul trebuie sa fie autorizat.
Acest lucru se face printr-un token JWT care este obtinut la intrarea in biblioteca.
Acest token este trimis in headerele cererii catre server si este verificat de acesta.

### Cereri

Cererile sunt realizate prin intermediul functiilor: compute_get_request, compute_post_request,
compute_delete_request. Acestea primesc ca parametrii URL-ul, payload-ul, headerele si
cookie-urile si returneaza un string care reprezinta cererea HTTP.

Acestea sunt trimise catre server prin intermediul functiei send_to_server care primeste
ca parametru socket file descriptor-ul si cererea si returneaza raspunsul primit de la server.

### Raspunsuri

Raspunsurile sunt primite de la server folosind functia receive_from_server,
apoi sunt parsate si afisate in functie de contextul actual.
In cazul in care se primeste o carte / lista de carti, acestea
sunt afisate in format JSON cu formatare pretty-print. Altfel, se afiseaza doar rezultatul
cererii: succes, esec (client) sau eroare interna a serverului.

### Tratarea erorilor

Erorile sunt tratate in functie de contextul actual, sau in functie de codul
de status primit de la server. In cazul in care se primeste un cod de status
care nu este 200, se va afisa un mesaj de eroare.

## Parson JSON

Pentru a putea lucra cu JSON, am folosit biblioteca [Parson](https://github.com/kgabis/parson).
Am ales aceasta biblioteca deoarece este simplu de folosit si are o documentatie
foarte buna. Este o biblioteca open-source si este foarte usor de integrat in proiect.

## Compilare si Rulare

Pentru a compila proiectul se va rula comanda `make`.

Pentru a rula proiectul se va rula comanda `./client`.

## Comenzi

### `register`

```bash
register
<username: String>
<password: String>
```

### `login`

```bash
login
<username: String>
<password: String>
```

### `enter_library`

```bash
enter_library
```

### `get_books`

```bash
get_books
```

### `get_book`

```bash
get_book
<id: Number>
```

### `add_book`

```bash
add_book
<title: String>
<author: String>
<genre: String>
<publisher: String>
<page_count: Number>
```

### `delete_book`

```bash
delete_book
<id: Number>
```

### `logout`

```bash
logout
```

### `exit`

```bash
exit
```

## Concluzii

In final, a fost un proiect challenging, dar interesant, deoarece
am invatat cum sa lucrez cu HTTP, JSON, JWT si cookies. Am invatat
cum sa fac cereri catre un server si cum sa parsez raspunsurile.
