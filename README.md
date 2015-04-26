# BriocheBot

Le bot qui envoie de la brioche !

Commandes disponibles:

	!brioche add <Pseudo Twitch> <Pseudo osu!> # Ajoute le joueur donné dans la liste des joueurs
	!brioche del <Pseudo Twitch> # Supprime le joueur donné de la liste des joueurs
	!brioche op <Pseudo Twitch> # Donne les droits d'admin au joueur donné
	!brioche deop <Pseudo Twitch> # Enlève les droits d'admin au joueur donné
	!brioche skin # Donne le skin du streameur actuel
	!brioche skin <Pseudo Twitch> # Donne le skin du joueur donné
	!brioche setskin <Skin osu!> # Remplace le skin du joueur ayant effectué la commande par celui donné
	!brioche stream # Place le joueur ayant effectué la commande en tant que streameur actuel de la chaîne
	!brioche restart # Relance le bot

Alias des commandes existantes:

	!status ... # Alias de !brioche stream. Utilisé avec Nightbot pour changer le titre du stream actuel, et compris par BriocheBot pour également changer le streameur.
	!skin # Alias de !brioche skin
	!skin <Pseudo Twitch> # Alias de !brioche skin <Pseudo Twitch>

A noter que certaines commandes ne peuvent être effectuées que par des administrateurs, d'autres que par des joueurs, et d'autres par tout le monde.
