#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <zmq.h>
#include "calc.h"

const  int msgsize = 100;

void transport(char *msgbuf, int msgsize) {

    int timeoutValue = 5000; // 5 secs

    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");
    zmq_send (requester, msgbuf, msgsize , 0);
    zmq_setsockopt( requester, ZMQ_RCVTIMEO, (char*)&timeoutValue, sizeof(timeoutValue));
    zmq_recv (requester, msgbuf, msgsize , 0);
    zmq_close (requester);
    zmq_ctx_destroy (context);  

    return;  
}

template <typename T> void encode( char *msgbuf, int msgsize, int op, T *Req ){
    
    XDR xdr;
    calc_req  req = {0};
    req.op = op;
    switch(req.op) {
        case ADD : 
                memcpy(&req.calc_req_u.add_request, Req, sizeof(T));
                break;
        case SUBTRACT : 
                memcpy(&req.calc_req_u.sub_request, Req, sizeof(T));
                break;
        case MULTIPLY : 
                memcpy(&req.calc_req_u.mul_request, Req, sizeof(T));
                break;
        case DIVIDE : 
                memcpy(&req.calc_req_u.div_request, Req, sizeof(T));
                break;
        case FACTORIAL : 
                memcpy(&req.calc_req_u.fac_request, Req, sizeof(T));
                break;
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

template <typename T>void decode( char *msgbuf, int msgsize, T *Res ){
    
    XDR xdr;
    calc_res res = {0};

    xdrmem_create ( &xdr, msgbuf, msgsize, XDR_DECODE);
    if (!xdr_calc_res (&xdr, &res)) {
        fprintf (stderr, "res: could not decode\n");
        exit (1);
    }
    switch(res.op) {
        case ADD : 
                memcpy(Res, &res.calc_res_u.add_result, sizeof(AddResult));
                break;
        case SUBTRACT: 
                memcpy(Res, &res.calc_res_u.sub_result, sizeof(SubResult));
                break;
        case MULTIPLY: 
                memcpy(Res, &res.calc_res_u.mul_result, sizeof(MulResult));
                break;
        case DIVIDE: 
                memcpy(Res, &res.calc_res_u.div_result, sizeof(DivResult));
                break;
        case FACTORIAL: 
                memcpy(Res, &res.calc_res_u.fac_result, sizeof(FacResult));
                break;
    }

    xdr_free((xdrproc_t) xdr_calc_res, (char *) &res);
    xdr_destroy(&xdr);

    return;
}

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

void test_subtract(void)
{
    SubWork Req;
    SubResult Res;

    Req.num1 = 23;
    Req.num2 = 13;

    char msgbuf[msgsize];
     
    encode( msgbuf, msgsize, SUBTRACT, &Req);
    transport( msgbuf, msgsize);
    decode( msgbuf, msgsize, &Res );

    assert( 0 == Res.err );
    assert( Req.num1 - Req.num2 == Res.num);
}

void test_multiply(void)
{
    MulWork Req;
    MulResult Res;

    Req.num1 = 3;
    Req.num2 = 4;
    Req.num3 = 2;

    char msgbuf[msgsize];
     
    encode( msgbuf, msgsize, MULTIPLY,  &Req);
    transport( msgbuf, msgsize);
    decode( msgbuf, msgsize, &Res );

    assert( 0 == Res.err );
    assert( Req.num1 * Req.num2 * Req.num3 == Res.num);
}

void test_divide(void)
{
    DivWork Req;
    DivResult Res;

    Req.num1 = 23;
    Req.num2 = 3;

    char msgbuf[msgsize];
     
    encode( msgbuf, msgsize, DIVIDE,  &Req);
    transport( msgbuf, msgsize);
    decode( msgbuf, msgsize, &Res );

    assert( 0 == Res.err );
    assert( Req.num1 / Req.num2  == Res.num);
}

void test_factorial(void)
{
    FacWork Req;
    FacResult Res;

    Req.num1 = 23;
    int factorial;

    for(int i=1; i<= Req.num1 ; ++i) factorial *= i;   

    char msgbuf[msgsize];
     
    encode( msgbuf, msgsize, FACTORIAL,  &Req);
    transport( msgbuf, msgsize);
    decode( msgbuf, msgsize, &Res );

    assert( 0 == Res.err );
    assert( factorial == Res.num);
}

int main () {

    test_add();
    test_subtract();
    test_multiply();
    test_divide();
    test_factorial();

    return 0;
}