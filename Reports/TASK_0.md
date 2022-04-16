# Se familiariser avec l'existant

## A- Exécution

> Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
>Sur quelle touche faut-il appuyer pour ajouter un avion ?

Il faut appuyer sur la touche c pour ajouter un avion.

> Comment faire pour quitter le programme ?

Pour quitter le programme on peut appuyer sur les touches 'Q' et 'X'.

>A quoi sert la touche 'F' ?

La touche 'F' sert à mettre la fenêtre en plein écran.

>Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?

Dans l'ordre :
- l'avion arrive dans la scène.
- il se pose sur la piste d'atterrissage.
- il prends des passagers.
- puis re-décolle et tourne en rond autour de l'aéroport avant de recommencé.

>Quelles informations s'affichent dans la console ?

Dans la console on affiche les actions de chaque avion. (il décolle, récupère des passagers ...)

>Ajoutez maintenant quatre avions d'un coup dans la simulation.
>Que fait chacun des avions ?

Ils vont chacun leur tour attendre qu'ils puissent récupérer des passagers (seulement s'il y a de la place) puis ils vont partir vers leur déstination pour revenir et répéter le cycle.

## B- Analyse du code

>Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.

| classe        | représentation |
| ------------- | -------------- |
| aircraft_type | représente les différents types d'avions, permets de stocker les informations propre à un avion |
| aircraft      | représente les à proprement parler un avion ainsi que ses actions et stock les informations de l'instance | 
| airport_type  | représente les informations d'un aéroports |
| airport       | représente à proprement parler un aéroport et contient les terminaux |
| geometry      | représente les actions possible sur les Point2D et Point3D du projet donc gère le calcul des poisitons d'affichage/déplacement |
| main          | permet de lancer la simulation |
| runaway       | représente le point dans l'espace qui indique aux avions qu'ils se sont éloignés de l'aéroport |
| terminal      | représente les terminaux de l'aéroport, permets aux avions de récupérer des passagers |
| tower_sim     | gère toute la simulations ainsi que les inputs de l'utilisateur |
| tower         | représente les tours de controle et fait le liens entre l'aéroport et les avions et dirige les avions |
| waypoint      | représente un point sur le chemin d'un avion |

>Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
>Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.

### Tower :

| fonction        | explication    |
| --------------- | -------------- |
| Tower() | c'est le constructor |
| get_instructions() | gère les actions que doit faire un avion selon s'il estsur le terminal de la ou non |
| arrived_at_terminal() | enclenche le service d'un avion une fois qu'il est arrivé au terminal |

### Aircraft :

| fonction        | explication    |
| --------------- | -------------- |
| Aircraft() | c'est le constructor |
| get_flight_num() | retourne le numéro de l'avion |
| distance_to() | retourne la distance entre l'avion et un point donné |
| display() | affiche la texture de l'avion |
| move() | déplace l'avion entre les waypoints |

### Airport :

| fonction        | explication    |
| --------------- | -------------- |
| Airport() | c'est le constructor |
| get_tower() | retourne la tour de contrôle |
| display() | affiche la texture de l'aéroport |
| move() | déplace les terminaux |

### Terminal :

| fonction        | explication    |
| --------------- | -------------- |
| Terminal() | c'est le constructor |
| in_use() | retourne si l'avion courant est null ou non |
| is_servicing() | retourne s'il y a un service en cours |
| assign_craft() | assigne un avion au terminal |
| start_service() | commence le service seulement si l'avion est en suffisament proche |
| finish_service() | désassigne l'avion une fois le service fini |
| move() | change le terminal si un avion y est assigné et qu'un service est en cours |

>Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?

Il y a :

- la classe Aircraft : 
    - arrive_at_terminal()
    - add_waypoint()
    - operate_landing_gear()

- la classe waypoint

- la classe tower :
    - get_circle()
    - get_instructions()

>Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
>Expliquez les intérêts de ce choix.

Le conteneur choisi est std::deque, comme l'avion va se déplacer entre plusieurs points différents, utiliser une deque plutôt qu'un vector est plus intéréssant car la deque permets un ajout et une suppression plus efficace, donc on va pouvoir ajouter et retirer des points sur la trajectoire de l'avion plus facilement.

## C- Bidouillons !

> Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.

Les vitesses maximales et accélération de chaque avion sont définis dans aircraft_types.hpp.

> Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.

```cpp
aircraft_types[2] = new AircraftType { /*.02f*/.05f,/*.05f*/ .08f, .02f, MediaPath { "concorde_af.png" } };
```

> Identifiez quelle variable contrôle le framerate de la simulation.

La variable controlant le framerate est la variable GL::ticks_per_sec dans opengl_interface.hpp.

> Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.

```cpp
GL::keystrokes.emplace('*', []() { GL::ticks_per_sec = std::min(GL::ticks_per_sec + 1u, 200u); });
GL::keystrokes.emplace('/', []() { GL::ticks_per_sec = std::max(GL::ticks_per_sec - 1u, 1u); });
```

ici il s'agit de la version final, on fait un min pour l'augmentation du framerate pour éviter de trop augmenter le framerate (la valeur 200u est complètement arbitraire), de plus on fait un max pour la réduction du framerate pour éviter de passer à 0 et faire crasher l'application.

> Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?

Le programme crash car division par 0.

> Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.

On va d'abord créer une nouvelle variable global dans opengl_interface.hpp nommé is_paused initialiser à false. puis on va modifier la fonction timer pour qu'elle prenne en compte la variable, ce qui nous donnes :

```cpp
void timer(const int step)
{
    if (!is_paused)
    {
        for (auto& item : move_queue)
        {
            item->move();
        }
        glutPostRedisplay();
    }
    glutTimerFunc(1000u / ticks_per_sec, timer, step + 1);
}
```

et dans tower_sim.cpp on vas ajouter un nouvel input qui va inverser la variable is_paused :

```cpp
GL::keystrokes.emplace('p', []() { GL::is_paused = !GL::is_paused; });
```

> Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.

La variable controlant le temps de débarquement des avions est la variable SERVICE_CYCLES dans config.hpp qui était à 20u et que l'on passe donc à 40u.

> Lorsqu'un avion a décollé, il réattérit peu de temps après. Faites en sorte qu'à la place, il soit retiré du programme.

> A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?

On peut savoir que l'avion doit être supprimer dans la fonction get_instruction de la classe Tower.

> Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ?

On ne peut pas retirer l'avion ici car on pourrait se retrouver avec des invalidations d'iterator.

> A quel endroit de la callstack pourriez-vous le faire à la place ?

On peut le faire dans la fonction Timer de la classe opengl_interface.

> Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?

Tout d'abord on va créer une variable dans la classe aircraft qu'on va appelé has_left et l'initialisé à false.

Ensuite, lorsqu'on est dans la fonction get_instruction, au moment ou l'avion doit être supprimé on va mettre cette variable à true.

Puis on va modifier la fonction move de l'avion pour que lorsque has_left est à true elle retourne false et on ne fait pas le reste de la fonction.

Pour que cela fonctionne on doit modifier la signature de move pour que cela retourne un booleén. Pour éviter les soucis, on doit modifier la signature de move pour la classe dynamic_objet et celles l'héritant (Pour toutes les classes filles on retournera juste true).

Une fois que c'est fait, dans la classe timer on vas remplacer la boucle for qui va appelé la fonction move de tout les items de move_queue par une boucle for iterator.

On va bouclé sur chaque item et si move renvoie true, on vas créer un pointer sur le dé-référencement de l'item et tout simplement retirer cette item de la liste et le delete (En faisant bien attention de ne pas perdre l'iterator).

Ce qui nous donne ceci (je montre juste la boucle for iterator qui est plus intéréssant) :

```cpp
for (auto it = move_queue.begin(); it != move_queue.end();)
{
    auto* item = *it;
    if (item->move())
    {
        ++it;
    }
    else
    {
        it = move_queue.erase(it);
        delete item;
    }
}
```

> Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.

```cpp
//DANS LA CLASSE DISPLAYABLE

...
Displayable(const float z_) : z { z_ } { display_queue.emplace_back(this); }
virtual ~Displayable()
{
    display_queue.erase(std::find(display_queue.begin(), display_queue.end(), this));
}
...
static inline std::vector<const Displayable*> display_queue;

```

De plus pour éviter les soucis, dans la fonction display de la classe opengl_interface, il faut faire des appelle static et non global à la liste display_queue, pour ce faire il faut juste préciser Displayable devant display_queue.

> Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction find_craft_and_terminal(const Aicraft&) ne devrait plus être nécessaire.

on utilisera la map suivante :

```cpp
std::map<const Aircraft*, size_t>;
```

## D- Théorie

> Comment a-t-on fait pour que seule la classe Tower puisse réserver un terminal de l'aéroport ?

on passe par la manipulation du champs `reserved_terminal` qui est privé et membres de la classe Tower. De plus, la classe Tower est une classe ami à Terminal.

> En regardant le contenu de la fonction void Aircraft::turn(Point3D direction), pourquoi selon-vous ne sommes-nous pas passer par une réference ?

Il n'est possible de passer par une const ref car on utilise la fonction cap_length du Point3D qui va venir le modifier. 

> Pourquoi n'est-il pas possible d'éviter la copie du Point3D passé en paramètre ?

On est obligé d'avoir le même Point3D au passage de la fonction car on ne veut pas le modifier si on le passe plusieurs fois dans la fonction.