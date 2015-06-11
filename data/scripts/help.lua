--------------------------------------------------------------------
--
-- Commande !help <Commande>
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command"

-- La commande
CommandString = "help"

-- Nombre minimum d'arguments
MinArguments = 0

-- Nombre maximum d'arguments
MaxArguments = 1

--------------------------------------------------------------------
--
-- Commande !help request
--
--------------------------------------------------------------------

local function helpRequest(senderNickname)

    server:sendTwitch("Le format des requêtes est: <Url> (+ <Commentaire>). Le commentaire est optionnel")

end

--------------------------------------------------------------------
--
-- Commande !help doc
--
--------------------------------------------------------------------

local function helpDoc(senderNickname)

    server:sendTwitch("Le doc est disponible à l'adresse http://tinyurl.com/osufrliverules. Il n'est accessible que par les streamers cependant")

end

--------------------------------------------------------------------
--
-- Commande !help nightbot
--
--------------------------------------------------------------------

local function helpNightbot(senderNickname)

    server:sendTwitch("Nightbot c'est mon grand frère. Mais il est moins bavard. Et il aime pas venir ici, il s'en va tout le temps.")

end

--------------------------------------------------------------------
--
-- Commande !help skin
--
--------------------------------------------------------------------

local function helpSkin(senderNickname)

    server:sendTwitch("Pour accéder au skin d'un joueur faites !brioche skin <Pseudo>, ou encore !skin <Pseudo>. Le pseudo est optionnel, et l'omettre donnera le skin du streamer actuel.")

end

--------------------------------------------------------------------
--
-- Commande !help
--
--------------------------------------------------------------------

local function help(senderNickname)

    server:sendTwitch("Les pages d'aide disponibles sont: request, doc, nightbot, skin. Faites !help <Page> pour y accéder.")

end

--------------------------------------------------------------------
--
-- Callback
--
--------------------------------------------------------------------

local pages =
{
    request = helpRequest,
    doc = helpDoc,
    nightbot = helpNightbot,
    skin = helpSkin
}

-- Callback
function onCommand(sender, page)

    -- Si aucune page n'est donnée
    if page == nil then

        -- On appelle la fonction sans argument
        help(sender)

    -- Sinon
    else

        -- Si la page demandée n'existe pas
        if pages[page] == nil then

            -- Erreur
            server:sendTwitch("La page " .. page .. " n'existe pas")

        -- Sinon
        else

            -- On appelle la fonction correspondante
            pages[page](sender)

        end

    end

end
