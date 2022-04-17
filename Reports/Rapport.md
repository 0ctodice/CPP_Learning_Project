> les choix auxquels vous avez été confrontés par rapport à l’architecture du programme, et les raisons qui ont fait que vous êtes parti dans une direction plutôt qu’une autre.

Le principal choix structurel que j'ai pu avoir a été au niveau de la mise en place de la suppression des aircrafts. Pour ce faire, j'ai fais évoluer leur fonction move afin de les faire retourner un booleen (true si l'avion existe toujours et false sinon). Bien évidemment, cette solution m'a forcé à modifier la fonction move de la classe mère (DynamicObject) et de ce fait modifié la fonction move de toutes les classes filles, pour ces dernières on les fait retourner true en toutes circonstances. Mais à part cela, je n'ai pas été confrontés à d'autres choix par rapport à l'architecture du programme, la précision du sujet et la qualité du cours m'ont permis de comprendre assez vite ce qui était attendu.

> les situations où vous avez bloqué, et si applicable, la solution que vous avez trouvée pour vous en sortir.

D'aussi loin que je me souvienne, je n'ai pas eu de blocage particulier. Du moins, rien qu'une relecture du sujet, du code et de la documentation ne puisse régler.

> ce que vous avez aimé et/ou détesté dans ce projet

J'ai beaucoup apprécié :
- le sujet du projet, le faite de passer par quelque chose d'aussi visuelle aide beaucoup au sentiment de récompense que l'on ressent lorsque notre code fonctionne.
- La redécouverte du C++ avec ces nouvelles fonctionnalités ont été aussi un vrai plaisir.
- Avoir un sujet bien précis dans ce qui est demandé fait beaucoup de bien.

Si je dois vraiment dire quelques que je n'ai pas aimés, je dirais d'être passé par github (ou git en général), je n'ai pas vraiment eu de cours dessus et j'ai failli perdre mon code à quelques reprises mais j'ai fini par dompter le tout.

> ce que vous pensez en avoir appris.

Je pense avoir appris à m'adapter selon le code d'autrui et le langage C++ en lui-même (notament l'utilisation des unique_ptr et de la libraire algorithme).
