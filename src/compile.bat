@echo off
echo Compiling sqlite3 (C code)...
gcc -c sqlite3.c -o sqlite3.o
if %errorlevel% neq 0 (
    echo sqlite3 compilation failed.
    pause
    exit /b
)

echo Compiling C++ source and linking...
g++ -std=c++17 -o banque.exe main.cpp adminIT.cpp caissier.cpp Client.cpp Compte.cpp CompteCourant.cpp CompteEpargne.cpp CompteManager.cpp CompteProfessionnel.cpp Employe.cpp EmployeClient.cpp InterfaceUtilisateur.cpp Manager.cpp ServiceAuthentification.cpp SystemeBancaire.cpp Transaction.cpp Utilisateur.cpp UtilisateurManager.cpp sqlite3.o -lsqlite3

if %errorlevel% neq 0 (
    echo C++ compilation or linking failed.
    pause
) else (
    echo Compilation successful.
)