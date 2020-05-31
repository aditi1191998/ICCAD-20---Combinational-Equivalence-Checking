module top(a ,b ,c ,d,e,x ,y );
output x ,y;
input a ,b ,c ,d,e;
wire w1 , w2 , w3 ;
and inst_1 ( w1 , a , b,e);
and inst_2 ( w2 , c , d );
or inst_3 ( w3 , w1 , w2 );
buf inst_4 ( x , w3 );
not inst_5 ( y , w3 );
endmodule

