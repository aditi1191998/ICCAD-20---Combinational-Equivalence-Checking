module _HMUX(O,I0,I1,S);
output O;
input I0,I1,S;
assign O = S?I1:I0;
endmodule
