
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <zmq.h>
#include "calc.h"

const int msgsize = 300;
void serve(char *request, int request_size);

int main (void)
{
    char msgbuf[msgsize];

    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    assert (rc == 0);

    while (1) {
        zmq_recv (responder, msgbuf, msgsize, 0);
        serve(msgbuf, msgsize);
        zmq_send (responder, msgbuf, msgsize, 0);
    }
    
    return 0;
}

void serve(char *request, int request_size) {
    XDR xdr_req;
    XDR xdr_res;

    calc_req  req = {0};
    calc_res  res = {0};

    // DECODE REQUEST
    xdrmem_create ( &xdr_req, request, request_size, XDR_DECODE );
    if (!xdr_calc_req (&xdr_req, &req)) {
        fprintf (stderr, "req: could not decode\n");
        exit (1);
    }
    xdr_free((xdrproc_t) xdr_calc_req, (char *) &req);
    xdr_destroy(&xdr_req);
  
    res.op = req.op;

    switch( req.op) {    
    case ADD : {
            AddWork Req;
            AddResult Res;
            memcpy( &Req, &req.calc_req_u.add_request, sizeof(AddWork));
            Res.num = Req.num1 +  Req.num2 +  Req.num3 + Req.num4 ; 
            printf("Res.num = %d (Add)\n", Res.num);
            Res.err = 0;
            memcpy( &res.calc_res_u.add_result, &Res, sizeof(AddResult)); 
            }
            break;
    case SUBTRACT : {
            SubWork Req;
            SubResult Res;
            memcpy( &Req, &req.calc_req_u.sub_request, sizeof(SubWork));
            Res.num = Req.num1 -  Req.num2 ; 
            printf("Res.num = %d (Sub)\n", Res.num);
            Res.err = 0;
            memcpy( &res.calc_res_u.sub_result, &Res, sizeof(SubResult)); 
            }
            break;
    case MULTIPLY : {
            MulWork Req;
            MulResult Res;
            memcpy( &Req, &req.calc_req_u.mul_request, sizeof(MulWork));
            Res.num = Req.num1 *  Req.num2 *  Req.num3 ; 
            printf("Res.num = %d (Multiply)\n", Res.num);
            Res.err = 0;
            memcpy( &res.calc_res_u.mul_result, &Res, sizeof(MulResult)); 
            }
            break;
    case DIVIDE : {
            DivWork Req;
            DivResult Res;
            memcpy( &Req, &req.calc_req_u.div_request, sizeof(DivWork));
            if ( Req.num2 != 0 ) {
                Res.num = Req.num1 / Req.num2 ; 
                printf("Res.num = %d (Divide)\n", Res.num);
                Res.err = 0;
            } else Res.err = -1;
            memcpy( &res.calc_res_u.div_result, &Res, sizeof(DivResult)); 
            }
            break;
    case FACTORIAL : {
            FacWork Req;
            FacResult Res;
            memcpy( &Req, &req.calc_req_u.fac_request, sizeof(FacWork));
            Res.num = 1;
            if ( Req.num1 > 1 ) {
                for (int c = 1; c <= Req.num1 ; c++) Res.num = Res.num * c; 
            }; 
            printf("Res.num = %d (Factorial)\n", Res.num);
            Res.err = 0;
            memcpy( &res.calc_res_u.fac_result, &Res, sizeof(FacResult)); 
            }
            break;
    }

    // ENCODE RESPONSE
    xdrmem_create ( &xdr_res, request, request_size, XDR_ENCODE);
    if (!xdr_calc_res (&xdr_res, &res)) {
        fprintf (stderr, "res: could not encode\n");
        exit (1);
    }

    xdr_free((xdrproc_t) xdr_calc_res, (char *) &res);
    xdr_destroy(&xdr_res);

    return;
}