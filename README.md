# IDATT2104---Project---Onion-router
<h2>Introduction</h2>

This is a simple onion router project written in C++. The project is a result of a collaboration between two students at NTNU. It is worth noting that they had little to no experience coding in C++, which resulted in a steep learning curve.
where they attempted to create an onion router. The project consists of a client which is possible to connect to on the browser
via localhost on a given port, nodes that are supposed to route requests between the user and the webserver, and a way to both
encrypt and decrypt these requests. For a detailed time sheet view: https://docs.google.com/spreadsheets/d/1uX4yp2hj6g2LPmyGWl_MSnyKShj7zwY4WwYC3pIKxBQ/edit?usp=sharing

For encryption/decryption we used:

    Diffie-Hellman - shared secret key between nodes
    AES_128 - encrypting and decrypting


<h3>Prerequisites for using the program:</h3>

    1. Use Windows (For winsock)
    2. Download OpenSSL from https://slproweb.com/products/Win32OpenSSL.html and choose Win64 OpenSSL, not the light version
    3. Install OpenSSL (view https://tecadmin.net/install-openssl-on-windows/)
    4. Make sure you have cmake installed

<h2>User Manual</h2>

Run the program and visit localhost:777/ for instructions.

<h3>Master branch</h3>
The master branch contains the program where the routing works. There is no encryption/decryption, but the get-request from the
client is sent through different nodes. A good testsite is http://localhost:777/www.softwareqatest.com/. It is important to end
the url with '/' if you are visiting a home page.

<h3>Node-with-enc</h3>
This branch contains the onion router. Sending the get-request from the client to the exit-node is usually
OK. This can be seen in the console where each string with the encrypted text is being printed. The request is
encrypted by the MainServer, before being sent to the ExitNode, where each encryption layer is being "peeled" by
each node it passes. NOTE! If the header gets corrupted the program will crash. Try again.

When receiving info from the page the user is trying to access, this is encrypted by each node on the way back, before
being completely decrypted by the MainServer and sent to the user. This data is often very corrupted, but some of
the data should be decrypted. 

The encryption/decryption is not reliable, so be prepared to run the program a couple of times to ensure that
you get the functionality described.

When testing this branch please visit http://localhost:777/www.softwareqatest.com/. This site should show the user
how the encryption/decryption works when reading the console.

<h2>Functionality:</h2>
<h3>Master Branch</h3>

    - Lets user connect to HTTP-sites where request is being passed through nodes
    - Client receives both HTML and CSS when requesting a webpage
    - Home page and help page
    - Basic error handling (404 NOT FOUND if wrong url)
    - Able to visit new pages without restarting the program

<h3>Node-with-enc</h3>
    
    - Encrypts get-request before being sent to exit node
        * Each node decrypts one layer before sending to next
    - The answer is being encrypted by each node until reached MainServer
        * MainServer decrypts each layer before sending it to user
    - Diffie-Hellman hanshake between nodes and keys also stored in MainServer for encryption/decryption
    
<h3>WinSocket</h3>

Windows lib for creating sockets in C++

https://docs.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock

<h3>OpenSSL</h3>
https://www.openssl.org/docs/man3.0/man3/

We sadly had some problems with the encryption/decryption in OpenSSL. For each run some of the data is being 
corrupted. It might have something to do with the secret keys, since different data is being corrupted each run.
There is also very limited documentation of the library. 

We used these classes from OpenSSL:
    
    - BIGNUM (for math with large numbers. Mainly key handshake) 
      https://www.openssl.org/docs/man1.0.2/man3/bn.html
    - BN_gen_rnd_prime (Used for getting random private keys that are prime)
    - EVP (AES)
      https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

<h3>Known bugs and weaknesses that should be improved upon in later versions:</h3>
    
    - Onion router doesn't work
    - Routing is not compatible with images.
    - If a page that contains images is loaded, the system fails.
    - Routing only works with some HTTP websites. The requirement for routing is that you can get the 
      ip of the HTTP-website with nslookup, paste the IP you get there into your browser and enter 
      there on port 80. E.g. www.datakom.no did not work during testing.
    - Can only serve one user at a time
    - The routing is slow
    - Nodes only run on localhost, and only 3 are used (with the same ip)
    - The port numbers of the Nodes are always the same
    - Encryption and decryption is quite unstable, with different words / 
      places in the string that gets corrupted each time (might have something to do with keygen)
    - The nodes are not being selected from a node pool
    - Not great memory management. Should be imporved



<h1>Task Description (NO)</h1>
Prosjektoppgaven er frivillig og kan kun utføres av studenter som får C i mappevurderingen i programmeringsdelen av emnet. Prosjektet gjennomføres av studenter som vil forbedre karakteren i programmeringsdelen til A eller B. Karakteren i programmeringsdelen teller 50% av totalkarakteren i emnet. Totalkarakteren vil alltid settes til gunst for studenten. En B i datakommunikasjonsdelen sammen med en C i programmeringsdelen vil altså gi totalkarakteren B.
Merk at alle deler av prosjektet (programmert løsning, dokumentasjon og presentasjon) forventes å holde meget god kvalitet for å oppnå karakteren B eller A.
Maks antall studenter i en gruppe: 3
Det forventes mer av løsningen jo flere man er på gruppen. Forventet innsats per student i en gruppe er 30-40 timer.
Oppgavetekst
Implementer onion routing enten som programvare eller programvarebibliotek i et valgfritt programmeringsspråk:

    Prioriter selv funksjonaliteter som skal inkluderes i løsningen.
        Det forventes ikke en komplett løsning
    Implementasjon i mer utfordrende programmeringsspråk som C++ eller Rust gir positiv uttelling på karakteren.
    Bruk av ferdige onion routing programvare eller biblioteker er ikke tillatt, men andre aktuelle programvarebiblioteker er tillatt
    Lag en README.md fil for løsningen. Ta med:
        Navn på løsningen og eventuell lenke til siste continuous integration/deployment kjøring
        Introduksjon
        Implementert funksjonalitet
        Fremtidig arbeid med oversikt over nåværende mangler/svakheter
Eksterne avhengigheter med en kort beskrivelse av hver avhengighet og hva den er brukt til
Installasjonsinstruksjoner
Instruksjoner for å bruke løsningen
Hvordan en kan kjøre eventulle tester
Eventuell lenke til API dokumentasjon

Bruk av ekstern informasjon/kode skal dokumenteres.
Innlevering
Innleveringsfristen er mandag 28. mars klokken 12:00. Lever løsningen på Inspera som en komprimert fil. Send epost til ole.c.eidheim@ntnu.no med en oversiktlig liste over gruppedeltakerne for å få tilgang til Inspera innleveringen.
Presentasjon
Hver presentasjon vil vare 15-20 minutter, med 5-10 minutter satt av til spørsmål fra faglærere. Eksempel innhold i presentasjonen kan være:

    Kort om løsningen, for eksempel:
        programmeringsspråk
        biblioteker brukt
        trådmodell (for eksempel en tråd per klient eller bruk av asynkrone kall og event loop)
    Implementert funksjonalitet, og eventuelle mangler/svakheter
    Demonstrasjon av løsningen

Oppmelding til presentasjonstidspunkt kommer senere.