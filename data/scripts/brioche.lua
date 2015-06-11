--------------------------------------------------------------------
--
-- Commande !brioche <commande> <arguments> <Pseudo Twitch> <Pseudo osu!>
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command"

-- La commande
CommandString = "brioche"

-- Nombre minimum d'arguments
MinArguments = 1

-- Nombre maximum d'arguments
MaxArguments = 4

--------------------------------------------------------------------
--
-- Commande !brioche add <Pseudo Twitch> <Pseudo osu!>
--
-- sender: Le pseudo de l'auteur de la commande
-- moderator: L'objet Moderator associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur à ajouter
-- osuUsername: Le pseudo osu! du joueur à ajouter
--
--------------------------------------------------------------------

local function briocheAdd(sender, moderator, twitchUsername, osuUsername)

    -- On tente de récupére le streamer qu'on veut ajouter
    local viewer = Viewer.get(twitchUsername)

    -- Si il existe
    if viewer ~= nil then

        -- Erreur
        server:sendTwitch("Le joueur " .. twitchUsername .. " existe déjà!")

        -- Sinon si on a les deux pseudos remplis
    elseif twitchUsername ~= nil and osuUsername ~= nil then

        -- On créé un nouveau streamer
        viewer = Viewer.newStreamer()

        -- On lui donne quelques infos
        viewer:setTwitchUsername(twitchUsername)
        viewer:setOsuUsername(osuUsername)
        viewer:setOsuSkinLink("null")

        -- Impossible de l'ajouter
        if not viewer:insert() then

            -- Erreur
            server:sendTwitch("Impossible d'ajouter " .. twitchUsername .. "/" .. osuUsername .. " en tant que streamer. Allez crier sur Soinou, c'est probablement de sa faute.")

            -- Et on supprime le streamer au passage
            viewer:free()

            -- Il est ajouté
        else

            -- On confirme à twitch
            server:sendTwitch("Streamer " .. twitchUsername .. "/" .. osuUsername .. " ajouté!")

        end
    end

end

--------------------------------------------------------------------
--
-- Commande !brioche edit <Pseudo Twitch> <Pseudo Twitch> <Pseudo osu!>
--
-- sender: Le pseudo de l'auteur de la commande
-- moderator: L'objet Player associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur à supprimer
-- newTwitchUsername: Le nouveau pseudo twitch du joueur
-- newOsuUsername: Le nouveau pseudo osu! du joueur
--
--------------------------------------------------------------------

local function briocheEdit(sender, moderator, twitchUsername, newTwitchUsername, newOsuUsername)

    -- On tente de récupére le viewer qu'on veut modifier
    local viewer = Viewer.get(twitchUsername)

    -- Si il n'existe pas
    if viewer == nil then

        -- Erreur
        server:sendTwitch("J'ai personne qui s'appelle " .. twitchUsername .. ". T'es sûr que t'as pas trop bu ?")

    -- Sinon
    else

        -- Si on a un nouveau pseudo twitch de précisé
        if newTwitchUsername ~= nil then

            -- On le mets à jour
            viewer:setTwitchUsername(newTwitchUsername)

        end

        -- Si on a un pseudo osu! de précisé et le viewer est au moins streamer/modérateur
        if newOsuUsername ~= nil and viewer:getType() ~= kViewer then

            -- On change son pseudo osu!
            viewer:setOsuUsername(newOsuUsername)

        end

        -- Impossible de le mettre à jour
        if not viewer:insert() then

            -- Erreur
            server:sendTwitch("J'ai pas réussi à changer le nom de " .. twitchUsername .. ". Doit y'avoir un bug caché, une fois de plus")

        -- Sinon si on a un pseudo osu!
        elseif newOsuUsername ~= nil then

            -- On confirme à twitch
            server:sendTwitch(twitchUsername .. " s'appelle maintenant  " .. newTwitchUsername .. "/" .. newOsuUsername .. ". Joyeuse nouvelle vie !")

        -- Pas de pseudo osu!
        else

            -- On confirme à twitch
            server:sendTwitch(twitchUsername .. " s'appelle maintenant " .. newTwitchUsername .. ". Joyeuse nouvelle vie !")

        end
    
    end

end

--------------------------------------------------------------------
--
-- Commande !brioche del <Pseudo Twitch>
--
-- sender: Le pseudo de l'auteur de la commande
-- moderator: L'objet Moderator associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur à supprimer
--
--------------------------------------------------------------------

function briocheDel(sender, moderator, twitchUsername)

    -- On récupère le streamer demandé
    local streamer = Viewer.get(twitchUsername)

    -- Il n'existe pas
    if streamer == nil then

        -- Erreur
        server:sendTwitch("Il existe pas " .. twitchUsername .. ". Tu veux supprimer un streamer qui existe pas maintenant ?")

    -- C'est l'envoyeur
    elseif moderator:is(twitchUsername) then

        -- Erreur
        server:sendTwitch("Tu veux te supprimer toi même ? T'es sûr ?")

    -- Sinon
    else

        -- On le supprime
        streamer.erase()

        -- On envoie un message à twitch
        server:sendTwitch("Adieu " .. twitchUsername .. ". ça aura été sympa de te connaître!")

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche op <Pseudo Twitch>
--
-- sender: Le pseudo de l'auteur de la commande
-- moderator: L'objet Moderator associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur à op
--
--------------------------------------------------------------------

function briocheOp(sender, moderator, twitchUsername)

    -- Pas de pseudo
    if twitchUsername == nil then
        
        -- wtf
        server:sendTwitch("Tu m'as pas donné de pseudo, tu veux que j'op comment ? J'lis pas dans les pensées hein")

    -- Un pseudo
    else

        -- On récupère le streamer demandé
        local streamer = Viewer.get(twitchUsername)

        -- Si il n'existe pas
        if streamer == nil then

            -- Non trouvé
            server:sendTwitch("Apprends à écrire un peu, " .. twitchUsername .. " il existe pas")

        -- Sinon si le joueur est déjà admin
        elseif streamer:getType() == kModerator then

            -- On envoie un message à twitch
            server:sendTwitch("Il est déjà modo " .. streamer:getTwitchUsername() .. ". J'ai pas de fonctionnalité double modo ou idk")

        -- Sinon
        else

            -- On le passe admin
            local newStreamer = streamer:setType(kModerator)

            -- Si y'a eu une erreur
            if newStreamer == nil then

                -- Nope
                server:sendTwitch("J'ai pas pu passer " .. twitchUsername .. " modo. Désolé :(")
        
            -- Sinon
            else

                -- On envoie un message à twitch
                server:sendTwitch("Et félicitations à " .. newStreamer:getTwitchUsername() .. " qui est maintenant modo. Tu vas pouvoir faire plein de trucs débiles!")

            end

        end

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche deop <Pseudo Twitch>
--
-- sender: Le pseudo de l'auteur de la commande
-- moderator: L'objet Moderator associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur à déop
--
--------------------------------------------------------------------

function briocheDeop(sender, moderator, twitchUsername)

    -- On récupère le streamer demandé
    local streamer = Viewer.get(twitchUsername)

    -- Si il n'existe pas
    if streamer == nil then

        -- Non trouvé
        server:sendTwitch("J'ai pas trouvé " .. twitchUsername .. ". Peut être une faute de frappe ?")

    -- Sinon si le streamer est l'envoyeur
    elseif moderator:is(twitchUsername) then

        -- Pas possible
        server:sendTwitch("Tu peux pas te déop toi-même, tu veux tout casser ou bien ?")

    -- Sinon
    else

        -- Pas possible de changer son type
        if not streamer:setType(kStreamer) then

            -- Erreur
            server:sendTwitch("Y'a eu une erreur, Soinou a encore dû casser un truc")

        -- Sinon
        else

            -- On envoie un message à twitch
            server:sendTwitch("Le joueur " .. streamer:getTwitchUsername() .. " n'est plus admin! Il pourra plus faire de conneries tiens")

        end

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche skin <Pseudo Twitch>
--
-- sender: Le pseudo de l'auteur de la commande
-- viewer: L'objet Viewer associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du streamer dont on veut le skin
--
--------------------------------------------------------------------

function briocheSkin(sender, streamer, twitchUsername)

    -- Variable temporaire pour le joueur
    local streamer = nil;

    -- Un argument
    if twitchUsername ~= nil then

        -- On récupère le joueur correspondant
        streamer = Viewer.get(twitchUsername)

    -- Aucun argument
    else

        -- On récupère le streamer actuel
        streamer = server:getCurrentStreamer()

    end

    -- Un pseudo twitch précisé mais le joueur n'est pas trouvé
    if twitchUsername ~= nil and streamer == nil then

        -- Erreur
        server:sendTwitch("Nope, " .. twitchUsername .. " existe pas. T'as du te tromper")

    -- Pas de pseudo twitch précisé mais le streamer n'existe pas non plus
    elseif streamer == nil then

        -- Pas de streamer
        server:sendTwitch("Y'a personne qui stream lààà. Attends un peu wesh")

    -- Pas de skin
    elseif streamer:getOsuSkinLink() == "null" then

        -- Erreur
        server:sendTwitch(streamer:getTwitchUsername() .. " il m'a pas filé de skin, donc je peux rien te donner")

    -- Tout est bon
    else

        -- On envoie le skin
        server:sendTwitch("Ok c'est bon j'ai changé le skin de " .. streamer:getTwitchUsername() .. " par " .. streamer:getOsuSkinLink())

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche setskin <Pseudo Twitch>
--
-- sender: Le pseudo de l'auteur de la commande
-- streamer/moderator: L'objet Streamer/Moderator associé à cet auteur (Ou nil)
-- osuSkinLink: Le nouveau skin du streamer
--
--------------------------------------------------------------------

function briocheSetSkin(sender, streamer, osuSkinLink)

    -- On change son skin
    streamer:setOsuSkinLink(osuSkinLink)

    -- On le mets à jour
    if not streamer:insert() then

        -- On affiche une erreur si c'est pas possible
        server:sendTwitch("Peut pas mettre à jour le skin de " .. streamer:getTwitchUsername() .. ". Encore un coup de Soinou ça")

    -- Si tout est bon
    else

        -- On indique la mise à jour
        server:sendTwitch("Nouveau skin de " .. streamer:getTwitchUsername() .. ": " .. osuSkinLink .. ". Espérons qu'il soit mieux que l'ancien")

    end

end

function adminBriocheSetSkin(sender, moderator, first, second)

    -- Si on a deux arguments et non un seul
    if second ~= nil then

        -- On récupère le joueur donné
        local streamer = Viewer.get(first)

        -- Si le joueur n'existe pas
        if streamer == nil then

            -- Erreur
            server:sendTwitch("Ahah tu t'es trompé, " .. first .. " il existe pas xD")

        -- Sinon
        else

            -- On appelle l'autre commande
            briocheSetSkin(sender, streamer, second)

        end

    -- Sinon
    else

        -- On appelle l'autre commande
        briocheSetSkin(sender, moderator, first)

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche stream <Pseudo Twitch>
--
-- sender: Le pseudo de l'auteur de la commande
-- streamer/moderator: L'objet Streamer/Moderator associé à cet auteur (Ou nil)
--
--------------------------------------------------------------------

function briocheStream(sender, streamer)

    -- On le définit en tant que streameur
    server:setCurrentStreamer(streamer)

    -- On envoie un message à twitch
    server:sendTwitch("Nouveau streameur: " .. streamer:getTwitchUsername() .. ". C'est parti \\o/")

end

function adminBriocheStream(sender, moderator, twitchUsername)

    -- Si on a un pseudo twitch
    if twitchUsername ~= nil then

        -- On récupère le streamer
        local streamer = Viewer.get(twitchUsername)

        -- Si le joueur n'existe pas
        if streamer == nil then

            -- Erreur
            server:sendTwitch("Il existe pas ton " .. twitchUsername .. ". Je sais pas où t'as trouvé ce nom, mais j'connais pas")

        -- Sinon
        else

            -- On appelle l'autre commande
            briocheStream(sender, streamer)

        end

    -- Sinon
    else

        -- On appelle la fonction normale
        briocheStream(sender, moderator)

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche stop
--
-- sender: Le pseudo de l'auteur de la commande
-- streamer: L'objet Streamer associé à cet auteur (Ou nil)
--
--------------------------------------------------------------------

function briocheStop(sender, streamer)

    -- On récupère le streamer actuel
    local currentStreamer = server:getCurrentStreamer()

    -- Personne
    if currentStreamer == nil then

        -- Erreur
        server:sendTwitch("Y'a personne qui stream, faut suivre un peu Kappa")

    -- Quelqu'un
    else

        -- Si le joueur est admin
        if streamer:getType() == kModerator then

            -- On récupère son temps de stream
            local hours, minutes, seconds = Utils.getTime(server:streamTime())

            -- On envoie un petit message à Twitch
            server:sendTwitch("Stream arrêté. ça faisait " .. hours .. " heure(s) " .. minutes .. " minute(s) " .. seconds .. " seconde(s) que " .. currentStreamer:getTwitchUsername() .. " streamait.");

            -- On vire le streamer actuel
            server:setCurrentStreamer(nil)

        -- Sinon
        else

            -- Si le streamer actuel est le joueur qui a fait la commande
            if currentStreamer:is(sender) then

                -- On récupère son temps de stream
                local hours, minutes, seconds = Utils.getTime(server:streamTime())

                -- On envoie un petit message à Twitch
                server:sendTwitch("Stream arrêté. ça faisait " .. hours .. " heure(s) " .. minutes .. " minute(s) " .. seconds .. " seconde(s) que tu streamais.");

                -- On vire le streamer actuel
                server:setCurrentStreamer(nil)

            end

        end

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche restart
--
-- sender: Le pseudo de l'auteur de la commande
-- moderator: L'objet Moderator associé à cet auteur (Ou nil)
--
--------------------------------------------------------------------

function briocheRestart(sender, moderator)

    -- Petit message à twitch
    server:sendTwitch("Adieu tout le monde :D")

    -- On stoppe le serveur (Il sera relancé automatiquement)
    server:stop()

end

--------------------------------------------------------------------
--
-- Callback
--
--------------------------------------------------------------------

-- La table des commandes accessibles aux moderateurs
local moderatorCommands =
{
    add = briocheAdd,
    del = briocheDel,
    edit = briocheEdit,
    op = briocheOp,
    deop = briocheDeop,
    skin = briocheSkin,
    setskin = adminBriocheSetSkin,
    stream = adminBriocheStream,
    stop = briocheStop,
    restart = briocheRestart
}

-- La table des commandes accessibles aux streamers
local streamerCommands =
{
    skin = briocheSkin,
    setskin = briocheSetSkin,
    stream = briocheStream,
    stop = briocheStop
}

-- La table des commandes accessibles aux viewers
local viewerCommands =
{
    skin = briocheSkin
}

-- Fonction permettant d'appeller une commande présente dans la table donnée
local function callCommand(command, table, ...)

    -- On récupère le callback correspondant à cette commande dans notre table
    local callback = table[command]

    -- Si la commande n'existe pas
    if callback == nil then

        -- Erreur
        server:sendTwitch("La commande !brioche " .. command .. " n'existe pas")

    -- Sinon
    else

        -- On appelle le callback
        callback(...)

    end

end

-- Callback
function onCommand(sender, command, ...)

    -- On récupère le viewer qui a envoyé le message
    local viewer = Viewer.get(sender)

    -- Si le viewer existe
    if viewer ~= nil then

        -- Si il est admin
        if viewer:getType() == kModerator then

            -- On appelle la commande depuis la table d'admin
            callCommand(command, moderatorCommands, sender, viewer, ...)

        -- Sinon si c'est un streamer
        elseif viewer:getType() == kStreamer then

            -- On appelle la commande depuis la table des joueurs
            callCommand(command, streamerCommands, sender, viewer, ...)

        -- Sinon si c'est un viewer
        elseif viewer:getType() == kViewer then

            -- On appelle la commande depuis la tables des viewers
            callCommand(command, viewerCommands, sender, viewer, ...)

        end

    -- Sinon
    else

        -- On appelle la commande depuis la table des viewers
        callCommand(command, viewerCommands, sender, viewer, ...)

    end

end
