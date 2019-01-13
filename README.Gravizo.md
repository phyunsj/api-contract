
# [Gravizo](http://www.gravizo.com/)

A simple way of describing graphs and include it easily in your web for free, blog, markdown page, github, and any location where remote images can be showed. Gravizo uses Graphviz to render graphs. It supports DOT, PlantUML, UMLGraph syntax and SVG in JSON format. It will include other formats in the future. No javascript, no plugins needed so you can include in any document. from _http://www.gravizo.com_

- [How to include graphviz graphs in github README](https://github.com/TLmaK0/gravizo)
- [Sequence Diagram Example](http://plantuml.com/sequence-diagram)
- [Having trouble using Gravizo? Cheat the system](https://gist.github.com/svenevs/ce05761128e240e27883e3372ccd4ecd)


## Example

```
![Alt text](https://g.gravizo.com/svg?
@startuml;
hide footbox;
autonumber;
actor User;
box "External Service";
participant "Proxy Service A" as A;
participant "Transport" as B;
end box;
box "Internal Service";
participant "Transport" as C;
participant "Service Handler" as D;
end box;
participant "Service A" as E;
... ...;
User -> A: Service Request;
activate A;
A -> B: Create Request/XDR encode;
note over A: switch req->op;
activate B;
B -> C: zmq_send;
activate C;
C -> D: XDR decode/Create Request;
activate D;
note over D;
switch req->op;
end note;
D -> E;
activate E;
D <--E;
deactivate E;
D --> C: XDR encode;
deactivate D;
C --> B: zmq_receive;
deactivate C;
B --> A: XDR decode;
deactivate B;
A --> User: Service Response;
deactivate A;
ref over A;
 test_add:calc_client.cpp;
end ref;
ref over D,E;
 serve:calc_server.cpp;
end ref;
...;
@enduml
)
```

```
 $ python
Type "help", "copyright", "credits" or "license" for more information.
>>> raw = '''  INSERT YOUR SEQUENCE DIAGRAM    '''
>>> import urllib
>>> urllib.quote(raw)
' ... USE THIS OUTPUT FOR GITHUB README ...'
```



## Diagram

![Alt text](https://g.gravizo.com/svg?%40startuml%3B%0Ahide%20footbox%3B%0Aautonumber%3B%0Aactor%20User%3B%0Abox%20%22External%20Service%22%3B%0Aparticipant%20%22Proxy%20Service%20A%22%20as%20A%3B%0Aparticipant%20%22Transport%22%20as%20B%3B%0Aend%20box%3B%0Abox%20%22Internal%20Service%22%3B%0Aparticipant%20%22Transport%22%20as%20C%3B%0Aparticipant%20%22Service%20Handler%22%20as%20D%3B%0Aend%20box%3B%0Aparticipant%20%22Service%20A%22%20as%20E%3B%0A...%20...%3B%0AUser%20-%3E%20A%3A%20Service%20Request%3B%0Aactivate%20A%3B%0AA%20-%3E%20B%3A%20Create%20Request/XDR%20encode%3B%0Anote%20over%20A%3A%20switch%20req-%3Eop%3B%0Aactivate%20B%3B%0AB%20-%3E%20C%3A%20zmq_send%3B%0Aactivate%20C%3B%0AC%20-%3E%20D%3A%20XDR%20decode/Create%20Request%3B%0Aactivate%20D%3B%0Anote%20over%20D%3B%0Aswitch%20req-%3Eop%3B%0Aend%20note%3B%0AD%20-%3E%20E%3B%0Aactivate%20E%3B%0AD%20%3C--E%3B%0Adeactivate%20E%3B%0AD%20--%3E%20C%3A%20XDR%20encode%3B%0Adeactivate%20D%3B%0AC%20--%3E%20B%3A%20zmq_receive%3B%0Adeactivate%20C%3B%0AB%20--%3E%20A%3A%20XDR%20decode%3B%0Adeactivate%20B%3B%0AA%20--%3E%20User%3A%20Service%20Response%3B%0Adeactivate%20A%3B%0Aref%20over%20A%3B%0A%20test_add%3Acalc_client.cpp%3B%0Aend%20ref%3B%0Aref%20over%20D%2CE%3B%0A%20serve%3Acalc_server.cpp%3B%0Aend%20ref%3B%0A...%3B%0A%40enduml)








