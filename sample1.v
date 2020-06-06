module top(a ,b ,c ,d,e,I0,I1,S,z,x ,y );
output x ,y,z;
input a ,b ,c ,d,e,I0,I1,S;
wire w1 , w2 , w3 ;
and inst_1 ( w1 , a , b,e);
and inst_2 ( w2 , c , d );
or inst_3 ( w3 , w1 , w2 );
buf inst_4 ( x , w3 );
not inst_5 ( y , w3 );
_HMUX inst_6 ( z,I0,I1,S );
_HMUX inst_6 ( z,I0,I1,S );
endmodule

