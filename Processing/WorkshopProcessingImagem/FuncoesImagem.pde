/*
 p = pixel a mudar
 f = cor de filtragem
 t = threshold
 */
color transparente(color p, color f, float t) {
  t = constrain(t, 0f, 441f);
  float d = dist((p>>16)&0xff, (p>>8)&0xff, (p>>0)&0xff, (f>>16)&0xff, (f>>8)&0xff, (f>>0)&0xff);

  if (d<t) {
    return color(p, 1);
  }
  return p;
}

/*
 p = pixel a mudar
 f = cor de filtragem
 t = threshold
 */
color pretoBranco(color p, color f, float t) {
  t = constrain(t, 0f, 441f);
  float d = dist((p>>16)&0xff, (p>>8)&0xff, (p>>0)&0xff, (f>>16)&0xff, (f>>8)&0xff, (f>>0)&0xff);

  if (d<t) {
    return color(0.2126*((p>>16)&0xff) + 0.71*((p>>8)&0xff) + 0.07*((p>>0)&0xff));
  }
  return p;
}

/*
 p = pixel a mudar
 f = cor de filtragem
 t = threshold
 */
color inverso(color p, color f, float t) {
  t = constrain(t, 0f, 441f);
  float d = dist((p>>16)&0xff, (p>>8)&0xff, (p>>0)&0xff, (f>>16)&0xff, (f>>8)&0xff, (f>>0)&0xff);

  if (d<t) {
    return color(255-((p>>16)&0xff), 255-((p>>8)&0xff), 255-((p>>0)&0xff));
  }
  return p;
}

/*
 p = pixel a mudar
 f = cor de filtragem
 t = threshold
 */
color preto(color p, color f, float t) {
  t = constrain(t, 0f, 441f);
  float d = dist((p>>16)&0xff, (p>>8)&0xff, (p>>0)&0xff, (f>>16)&0xff, (f>>8)&0xff, (f>>0)&0xff);
  int dec = (millis()/5)%256;

  if (d<t) {
    return color(max(0,((p>>16)&0xff)-dec), max(0,((p>>8)&0xff)-dec), max(0,((p>>0)&0xff)-dec));
  }
  return p;
}

/*
 p = pixel a mudar
 f = cor de filtragem
 t = threshold
 */
color branco(color p, color f, float t) {
  t = constrain(t, 0f, 441f);
  float d = dist((p>>16)&0xff, (p>>8)&0xff, (p>>0)&0xff, (f>>16)&0xff, (f>>8)&0xff, (f>>0)&0xff);
  int inc = (millis()/5)%256;

  if (d<t) {
    return color(min(255,((p>>16)&0xff)+inc), min(255,((p>>8)&0xff)+inc), max(255,((p>>0)&0xff)+inc));
  }
  return p;
}

/*
 p = pixel a mudar
 f = cor de filtragem
 t = threshold
 */
color arcoIris(color p, color f, float t) {
  t = constrain(t, 0f, 441f);
  float d = dist((p>>16)&0xff, (p>>8)&0xff, (p>>0)&0xff, (f>>16)&0xff, (f>>8)&0xff, (f>>0)&0xff);
  color r = color(0);

  int sC = (int)d + millis();
  sC = 255-(sC%256);

  if (sC < 85) {
    r = color(255 - sC * 3, 0, sC * 3);
  } else if (sC < 170) {
    sC -= 85;
    r = color(0, sC * 3, 255 - sC * 3);
  } else {
    sC -= 170;
    r = color(sC * 3, 255 - sC * 3, 0);
  }

  if (d<t) {
    return r;
  }
  return p;
}

/*
 p = pixel a mudar
 f = cor de filtragem
 c = cor resultado
 t = threshold
 */
color substituirCor(color p, color f, color c, float t) {
  t = constrain(t, 0f, 441f);
  float d = dist((p>>16)&0xff, (p>>8)&0xff, (p>>0)&0xff, (f>>16)&0xff, (f>>8)&0xff, (f>>0)&0xff);

  if (d<t) {
    return c;
  }
  return p;
}
