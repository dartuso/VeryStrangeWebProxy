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

INFO:
The purpose of this assignment is to learn about the HyperText Transfer Protocol (HTTP) used by the World Wide Web. In particular, you will design and implement an HTTP proxy (i.e., Web proxy) with functionality that demonstrates both the simplicity and the power of HTTP as an application-layer protocol. Along the way, you will also learn a lot about socket programming, TCP/IP, network debugging, and more.

Preamble
The so-called "Fake News" phenomenon is rampant on the Internet, but it seems unfair to let Donald Trump have all the fun, so let's make some fake news of our own! We are going to make a Web proxy that alters certain content on simple Web pages before they are rendered by the Web browser, so that the user sees factually incorrect information without knowing it. To keep the assignment simple, we will restrict ourselves only to HTTP (not HTTPS), and consider only basic text and HTML pages with a few images.

For our purpose, the fake news will involve my friend Floppy from Italy. Specifically, you need to change all occurrences of "Floppy" on a Web page into "Trolly", and all occurrences of "Italy" into "Japan". And if you find any JPG images of Floppy (linked or embedded), then you should replace them with your favourite troll image file (JPG, GIF, or PNG) from the Internet. Woot!!

Background
A Web proxy is a piece of software that functions as an intermediary between a Web client (browser) and a Web server. The Web proxy intercepts Web requests from clients and reformulates the requests for transmission to a Web server. When a response is received from the Web server, the proxy sends the response back to the client. From the server's point of view, the proxy is the client, since that is where the request comes from. Similarly, from the client's point of view, the proxy is the server, since that is where the response comes from. A Web proxy thus provides a single point of control to regulate Internet access between clients and servers. A lot of Calgary schools use Web proxies to limit the types of Web sites that students are allowed to access. Net Nanny and Barracuda are examples of commercially available Web proxies.

Technical Requirements
In this assignment, you will implement your very own Web proxy, in either C or C++. The goals of the assignment are to build a properly functioning Web proxy for simple Web pages, and then use your proxy to change some of the content before it is delivered to the browser.

There are two main pieces of functionality needed in your proxy. The first is the ability to intercept (and parse) HTTP requests and responses, so that the proxy can determine what changes (if any) need to be made to the requested content. The second is the ability to insert the false information into the page in some appropriate way so that the page still renders properly. Think of the fun you can have by trolling your unsuspecting roommate with fake news via your proxy. Hahahahaha!!!

The most important HTTP command for your Web proxy to handle is the "GET" request, which specifies the URL for an object to be retrieved. In the basic operation of your proxy, it should be able to parse, understand, and forward to the Web server a (possibly modified) version of the client HTTP request. Similarly, the proxy should be able to parse, understand, and return to the client a (possibly modified) version of the HTTP response that the Web server provided to the proxy. Please give some careful thought to how your proxy handles commonly occurring HTTP response codes, such as 200 (OK), 206 (Partial Content), 301 (Moved Permanently), 302 (Found), 304 (Not Modified), 403 (Forbidden), and 404 (Not Found).

You will need at least one TCP socket (i.e., SOCK_STREAM) for client-proxy communication, and at least one additional TCP socket for each Web server you are talking to for proxy-server communication. (If you want your proxy to support multiple concurrent HTTP transactions, you will need to fork child processes for request handling as well. See optional bonus part below.) Each child process or thread will use its own socket instances for its communications with the client and with the server.

When implementing your proxy, feel free to compile and run your Web proxy on any suitable department machine, or even your home machine or laptop, but please be aware that you will ultimately have to demo your proxy to your TA on campus at some point. You should try to access your proxy from your favourite Web browser (e.g., Internet Explorer, Mozilla Firefox, Chrome, Safari), and computer (either on campus or at home). To test the proxy, you will have to configure your Web browser to use your specific Web proxy (e.g., look for menu selections like Tools, Internet Options, Proxies, Advanced, LAN Settings).

As you design and build your Web proxy, give careful consideration to how you will debug and test it. For example, you may want to print out information about requests and responses received, processed, forwarded, redirected, or altered. Once you become confident with the basic operation of your Web proxy, you can toggle off the verbose debugging output. If you are testing on your home network, you can also use tools like WireShark or tcpdump to collect network packet traces. By studying the HTTP messages and TCP/IP packets going to and from your proxy, you might be able to figure out what is working, what isn't working, and why.

When you are finished, please submit your solution in electronic form to your TA via D2L. Your submission should include the source code for your Web proxy, a brief user manual describing how to compile and use your proxy, and a description of the testing done with your proxy. Please remember that assignments are to be done individually, and submitted to your assigned TA on time. You should also plan to give a brief demo of your proxy to your TA in February, either just before or just after your submission.
