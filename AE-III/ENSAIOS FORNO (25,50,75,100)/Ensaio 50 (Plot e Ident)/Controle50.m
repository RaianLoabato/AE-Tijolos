%Controle
s= tf ('s');
G = 0.2207/(s + 0.0001859);
rltool(G)

step(G)