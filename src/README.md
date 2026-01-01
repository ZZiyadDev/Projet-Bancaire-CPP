# Projet Banque

This project is a simple banking application written in C++.

## Compilation

To compile the project, you can use the `compile.bat` script.

```bash
.\compile.bat
```

Alternatively, you can compile the project manually using g++:

```bash
g++ -std=c++17 -o banque.exe main.cpp adminIT.cpp caissier.cpp Client.cpp Compte.cpp CompteCourant.cpp CompteEpargne.cpp CompteManager.cpp CompteProfessionnel.cpp Employe.cpp EmployeClient.cpp InterfaceUtilisateur.cpp Manager.cpp ServiceAuthentification.cpp SystemeBancaire.cpp Transaction.cpp Utilisateur.cpp UtilisateurManager.cpp sqlite3.o -lsqlite3
```

## Login

You can log in as an admin using the following credentials:

-   **Username:** EMP001
-   **Password:** admin

## Debugging

**I need your help to debug the application.**

I have reverted the changes to the inheritance structure.
Now, the debug output in `UtilisateurManager.cpp` will show the address of the `unique_ptr` objects. This will help us verify if the objects are correctly stored in the vector.

Please follow these steps:

1.  Compile and run the application.
2.  Attempt to log in as admin.
3.  Copy the **entire** console output, from the moment you run the application until it asks for input again. Pay close attention to messages starting with `[UtilisateurManager]`, `Loaded user:`, `[DEBUG] Utilisateurs loaded from DB`, `[DEBUG]   - ID: ... (Address: ...)`, and `Type d'utilisateur:`.
4.  Paste the entire console output in your next message.

This will provide me with the necessary information to understand what is happening.

