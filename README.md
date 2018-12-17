# API Contract

> :book: **Definition** A machine readable definition of an API interface, using a common format like API Blueprint, Swagger or RAML, providing a definition of the surface area of the resources that are available via API. API definitions are proving to be very useful in establishing a common way to describe, communicate and collaborate around APIs, which are often extremely abstract. ..._from [apievangelist.com](https://apievangelist.com/2014/07/15/an-api-definition-as-the-truth-in-the-api-contract/)_ 

## Example : [XDR as an API Contract](https://github.com/phyunsj/api-contract/tree/master/xdr_zmq_calc)

#### [Interface Definition : XDR](https://github.com/phyunsj/api-contract/blob/master/xdr_zmq_calc/calc.x) 

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
    transport( msgbuf, msgsize);
    decode( msgbuf, msgsize, &Res );

    assert( 0 == Res.err );
    assert( Req.num1 + Req.num2 + Req.num3 + Req.num4 == Res.num);
}
```

## Related Posts

- [Use Swagger to define RESTful APIs](https://developer.ibm.com/articles/wa-use-swagger-to-document-and-define-restful-apis/)
- [gRPC](https://grpc.io/)
- [Apache Thrift](https://thrift.apache.org/)
- [facebook Thrift](https://github.com/facebook/fbthrift) , aka FBThrift (i.e, [FBOSS Agent](https://github.com/facebook/fboss/tree/master/fboss/agent) )
- [Apache Avro](https://avro.apache.org/)
