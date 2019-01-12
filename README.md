# API Contract

> :book: **Definition** A machine readable definition of an API interface, using a common format like API Blueprint, Swagger or RAML, providing a definition of the surface area of the resources that are available via API. API definitions are proving to be very useful in establishing a common way to describe, communicate and collaborate around APIs, which are often extremely abstract. ..._from [apievangelist.com](https://apievangelist.com/2014/07/15/an-api-definition-as-the-truth-in-the-api-contract/)_ 

## Example : [XDR as an API Contract](https://github.com/phyunsj/api-contract/tree/master/xdr_zmq_calc)

#### [Interface Definition : calc.x](https://github.com/phyunsj/api-contract/blob/master/xdr_zmq_calc/calc.x) 

```
const ADD       = 601;
const SUBTRACT  = 602;
... // omitted for brevity

struct AddWork {
  int num1;
  int num2;
  int num3;
  int num4;
};

struct AddResult {
  int num;
  int err;
};
...// omitted for brevity
union calc_req switch ( int op ) {
    case ADD       : AddWork add_request;
    case SUBTRACT  : SubWork sub_request;
    ... // omitted for brevity
};

union calc_res switch ( int op ) {
    case ADD       : AddResult add_result;
    case SUBTRACT  : SubResult sub_result;
    ... // omitted for brevity
};
```

![Alt text](https://g.gravizo.com/svg?%40startuml%3B%0Ahide%20footbox%3B%0Aautonumber%3B%0Aactor%20User%3B%0Abox%20%22External%20Service%22%3B%0Aparticipant%20%22Proxy%20Service%20A%22%20as%20A%3B%0Aparticipant%20%22Transport%22%20as%20B%3B%0Aend%20box%3B%0Abox%20%22Internal%20Service%22%3B%0Aparticipant%20%22Transport%22%20as%20C%3B%0Aparticipant%20%22Service%20Handler%22%20as%20D%3B%0Aend%20box%3B%0Aparticipant%20%22Service%20A%22%20as%20E%3B%0A...%20...%3B%0AUser%20-%3E%20A%3A%20Service%20Request%3B%0Aactivate%20A%3B%0AA%20-%3E%20B%3A%20Create%20Request/XDR%20encode%3B%0Anote%20over%20A%3A%20switch%20req-%3Eop%3B%0Aactivate%20B%3B%0AB%20-%3E%20C%3A%20zmq_send%3B%0Aactivate%20C%3B%0AC%20-%3E%20D%3A%20XDR%20decode/Create%20Request%3B%0Aactivate%20D%3B%0Anote%20over%20D%3B%0Aswitch%20req-%3Eop%3B%0Aend%20note%3B%0AD%20-%3E%20E%3B%0Aactivate%20E%3B%0AD%20%3C--E%3B%0Adeactivate%20E%3B%0AD%20--%3E%20C%3A%20XDR%20encode%3B%0Adeactivate%20D%3B%0AC%20--%3E%20B%3A%20zmq_receive%3B%0Adeactivate%20C%3B%0AB%20--%3E%20A%3A%20XDR%20decode%3B%0Adeactivate%20B%3B%0AA%20--%3E%20User%3A%20Service%20Response%3B%0Adeactivate%20A%3B%0Aref%20over%20A%3B%0A%20test_add%3Acalc_client.cpp%3B%0Aend%20ref%3B%0Aref%20over%20D%2CE%3B%0A%20serve%3Acalc_server.cpp%3B%0Aend%20ref%3B%0A...%3B%0A%40enduml)

#### [Service Request](https://github.com/phyunsj/api-contract/blob/master/xdr_zmq_calc/calc_client.cpp) : ZMQ as Transport Layer

```
void test_add(void)
{
    AddWork Req;
    AddResult Res;

    Req.num1 = 3;
    Req.num2 = 13;
    Req.num3 = 25;
    Req.num4 = 77;

    char msgbuf[msgsize];
     
    encode( msgbuf, msgsize, ADD, &Req);
    transport( msgbuf, msgsize); // ZMQ
    decode( msgbuf, msgsize, &Res );

    assert( 0 == Res.err );
    assert( Req.num1 + Req.num2 + Req.num3 + Req.num4 == Res.num);
}
```

#### [En(De)coder](https://github.com/phyunsj/api-contract/blob/master/xdr_zmq_calc/calc_client.cpp)

```
void encode( char *msgbuf, int msgsize, int op, T *Req ){
    
    XDR xdr;
    calc_req  req = {0};
    req.op = op;
    switch(req.op) {
        case ADD : 
                memcpy(&req.calc_req_u.add_request, Req, sizeof(T));
                break;
        case SUBTRACT : 
                ...
     }
     
     xdrmem_create (&xdr, msgbuf, msgsize , XDR_ENCODE);
     if (!xdr_calc_req (&xdr, &req)) {
        fprintf (stderr, "req: could not encode\n");
        exit (1);
    }
    xdr_free((xdrproc_t) xdr_calc_req, (char *) &req);
    xdr_destroy(&xdr);
    return;
 }
 
 void decode( char *msgbuf, int msgsize, T *Res ){
     
    XDR xdr;
    calc_res res = {0};

    xdrmem_create ( &xdr, msgbuf, msgsize, XDR_DECODE);
    if (!xdr_calc_res (&xdr, &res)) {
        fprintf (stderr, "res: could not decode\n");
        exit (1);
    }
    switch(res.op) {
        case ADD : 
                memcpy(Res, &res.calc_res_u.add_result, sizeof(T));
                break;
        case SUBTRACT: 
                ...
                
     }
     xdr_free((xdrproc_t) xdr_calc_res, (char *) &res);
     xdr_destroy(&xdr);
}
```

## Related Posts

- [Swagger to define RESTful APIs](https://developer.ibm.com/articles/wa-use-swagger-to-document-and-define-restful-apis/)
- [gRPC](https://grpc.io/)
- [Apache Thrift](https://thrift.apache.org/)
- [facebook Thrift](https://github.com/facebook/fbthrift) , aka FBThrift (i.e, [FBOSS Agent](https://github.com/facebook/fboss/tree/master/fboss/agent) )
- [Apache Avro](https://avro.apache.org/)
