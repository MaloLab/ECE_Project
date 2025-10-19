// Définir les polynômes initiaux et les paramètres
s = poly(0, 's');
z = poly(0, 'z');
M = 0.025;
m = 0.092;
b = 0.5;
I = 0.00011;
g = 9.8;
l = 0.035;
q = (M + m) * (I + m * l^2) - (m * l)^2;

// Définir la fonction de transfert continue
numG = (m * l / q) * s;
denG = s^3 + (b * (I + m * l^2) / q) * s^2 - ((M + m) * m * g * l / q) * s - (b * m * g * l / q);

G = syslin('c', numG / denG);

// Convertir en représentation espace-état
Gss = tf2ss(G);

// Convertir en système discret avec un échantillonnage Te
Te = 0.001;
sl1 = cls2dls(Gss, Te);

// Convertir le système discret espace-état en fonction de transfert discrète
Gz = ss2tf(sl1);
disp(Gz);
