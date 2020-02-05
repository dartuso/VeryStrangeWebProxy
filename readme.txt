Assignment 1: Fake News Proxy
Daniel Artuso - 30029826
This is a simple web proxy that forwards user requests to the desired web address 
and returns the content provided back to the user with modifications.

Compliation
run
g++ .cpp -o proxy && proxy (optional port number)

or
make

if port number not specified it is defaulted to 8080

Configure 
ON command line provide the desired port to listen at:
./proxy 1457
The proxy will listen at 1457 for client requests

Use
Set proxy on system or web browser to ip:port that proxy is run at or 127.0.0.1:port for locally run.
Browse the web as desired 

Features:
Changes Floppy to Trolly
Changes Italy to Germany
Changes images with Floppy in url to a troll face
Work from basic txt files to more complicated pages with html and images
