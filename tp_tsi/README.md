# TP d'introduction à l'informatique graphique

TP tutoriel pour OpenGL, [TP original par Damien Rohmer et David Odin](https://imagecomputing.net/damien.rohmer/teaching/2015_2016/semestre_1/4eti_tsi/index.html), mis à jour par Thibault Dupont

## Compilation

Pour compiler et executer à partir du CMakeLists.txt en ligne de commande (Linux et macOs)

```sh
mkdir build
cd build
cmake ..
make
cd ..
./build/programme_n  ( avec n le numero du programme souhaité )
```

ou 

```sh
mkdir build
cmake . -B build
make -C ./build && ./build/programme_n
```

**Testé sur windows avec Visual Studio Community 2019 (MSVC2019)**

**Note sur l'utilisation des IDE (QtCreator, etc)**

Le repertoire d'execution doit être tp_tsi.
C'est a dire que le repertoire data/ doit être accessible.         

Image à usage libre : https://www.pexels.com/photo/boston-terrier-wearing-unicorn-pet-costume-1564506/