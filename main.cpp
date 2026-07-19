#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main() {
    // 1. Création de la fenêtre graphique
    sf::RenderWindow window(sf::VideoMode({600, 500}), "Nerdcore Soundboard Custom UI");
    window.setFramerateLimit(60);

    // 2. Chargement de la police pour le texte
    sf::Font font;
    if (!font.openFromFile("videotype.ttf")) {
        std::cout << "Erreur : Impossible de charger videotype.ttf !" << std::endl;
        return -1;
    }

    // 3. Scan automatique du dossier musiques
    std::vector<std::string> listeMusiques;

    // On crée une liste de rectangles pour nos boutons
    std::vector<sf::RectangleShape> listeBoutons;

    std::vector<sf::Text> listeTextes;

    if (std::filesystem::exists("musique")) {
        for (const auto& entry : std::filesystem::directory_iterator("musique")) {
            if (entry.path().extension() == ".ogg" || entry.path().extension() == ".wav") {
                listeMusiques.push_back(entry.path().filename().string());
            }
        }
    }

    int offsetHoriz = 0; // Variable pour gérer le décalage horizontal des boutons
    int offsetVert = 0; // Variable pour gérer le décalage vertical des boutons
    for (size_t i = 0; i < listeMusiques.size(); ++i) {
        sf::RectangleShape bouton(sf::Vector2f(400.f, 35.f));
        sf::Text txtMusique(font, listeMusiques[i], 14);
        if (i % 5 == 0 && i != 0) {
            // Si on a déjà 5 boutons sur la colonne, on passe à la colonne suivante
            offsetHoriz += 420; // Décalage horizontal
            offsetVert = 0; // Réinitialiser le décalage vertical
        }

        bouton.setPosition({100.f + offsetHoriz, 100.f + offsetVert * 50.f});
        bouton.setFillColor(sf::Color(50, 50, 80));
        bouton.setOutlineThickness(2.f);
        bouton.setOutlineColor(sf::Color::Blue);
        
        // Texte sur le bouton (nom de la musique)
        txtMusique.setFillColor(sf::Color::White);
        txtMusique.setPosition({110.f + offsetHoriz, 108.f + offsetVert * 50.f});
        

        offsetVert++;
        
        // On ajoute ce bouton dans notre liste
        listeBoutons.push_back(bouton);

        listeTextes.push_back(txtMusique);
    }

    

    sf::Music musiqueActuelle;
    std::string chansonEnCours = "Aucune";
    float ratio = 0.0f;

    // 4. Boucle principale de la fenêtre
        // 4. Boucle principale de la fenêtre (Version SFML 3)
        
    // Dessin des boutons permanent
    
    sf::RectangleShape btnPrevious(sf::Vector2f(130.f, 40.f));
    btnPrevious.setPosition({10.f, 420.f});
    btnPrevious.setFillColor(sf::Color(20, 120, 20));

    sf::RectangleShape btnStop(sf::Vector2f(130.f, 40.f));
    btnStop.setPosition({320.f, 420.f});
    btnStop.setFillColor(sf::Color(120, 20, 20));

    sf::RectangleShape barreProgression(sf::Vector2f(400.f, 20.f));
    barreProgression.setPosition({100.f, 380.f});
    barreProgression.setFillColor(sf::Color(50, 50, 80));

    sf::RectangleShape btnNext(sf::Vector2f(130.f, 40.f));
    btnNext.setPosition({490.f, 420.f});
    btnNext.setFillColor(sf::Color(20, 120, 20));

    while (window.isOpen()) {
        
        // Boucle d'événements moderne avec std::optional
        while (const std::optional event = window.pollEvent()) {
            
            // Détection du redimensionnement de la fenêtre (SFML 3)
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                // 1. On récupère la nouvelle taille de la fenêtre
                sf::Vector2u nouvelleTaille = resized->size;

                // 2. On crée une nouvelle vue qui correspond EXACTEMENT à cette taille
                sf::View vue(sf::FloatRect({0.f, 0.f}, sf::Vector2f(nouvelleTaille)));

                // 3. On applique cette vue à notre fenêtre
                window.setView(vue);
            }

            // 1. Détection de la fermeture de la fenêtre
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // 2. Détection du clic de souris
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                // On vérifie qu'il s'agit bien du clic gauche
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    
                    // Récupérer la position de la souris lors du clic
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Vérifier le clic sur les boutons de musique
                    for (size_t i = 0; i < listeBoutons.size(); ++i) {
                        if (listeBoutons[i].getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                            musiqueActuelle.stop();
                            if (musiqueActuelle.openFromFile("musique/" + listeMusiques[i])) {
                                musiqueActuelle.play();
                                chansonEnCours = listeMusiques[i];
                            }
                        }
                    }

                    if (btnPrevious.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                        for (size_t i = 0; i < listeMusiques.size(); ++i) {
                            if (chansonEnCours == listeMusiques[i]) {
                                musiqueActuelle.stop();
                                if (i > 0) {
                                    if (musiqueActuelle.openFromFile("musique/" + listeMusiques[i - 1])) {
                                        musiqueActuelle.play();
                                        chansonEnCours = listeMusiques[i - 1];
                                    }
                                } else {
                                    if (musiqueActuelle.openFromFile("musique/" + listeMusiques[listeMusiques.size() - 1])) {
                                        musiqueActuelle.play();
                                        chansonEnCours = listeMusiques[listeMusiques.size() - 1];
                                    }
                                }
                                break;
                            }
                        }
                    }
                    // Bouton PAUSE (Position : x=150 à 280, y=420 à 460)
                    if (mousePos.x >= 150 && mousePos.x <= 280 && mousePos.y >= 420 && mousePos.y <= 460) {
                        if (musiqueActuelle.getStatus() == sf::SoundSource::Status::Playing) musiqueActuelle.pause();
                        else if (musiqueActuelle.getStatus() == sf::SoundSource::Status::Paused) musiqueActuelle.play();
                    }

                    // Bouton STOP (Position : x=320 à 450, y=420 à 460)
                    if (btnStop.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                        musiqueActuelle.stop();
                        chansonEnCours = "Aucune";
                    }
                    
                    // Bouton NEXT (Position : x=490 à 620, y=420 à 460)
                    if (btnNext.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                        for (size_t i = 0; i < listeMusiques.size(); ++i) {
                            if (chansonEnCours == listeMusiques[i]) {
                                musiqueActuelle.stop();
                                if (i + 1 < listeMusiques.size()) {
                                    if (musiqueActuelle.openFromFile("musique/" + listeMusiques[i + 1])) {
                                        musiqueActuelle.play();
                                        chansonEnCours = listeMusiques[i + 1];
                                    }
                                } else {
                                    if (musiqueActuelle.openFromFile("musique/" + listeMusiques[0])) {
                                        musiqueActuelle.play();
                                        chansonEnCours = listeMusiques[0];
                                    }
                                }
                                break;
                            }
                        }
                    }

                    if (barreProgression.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                        float newRatio = (mousePos.x - barreProgression.getPosition().x) / barreProgression.getSize().x;
                        if (newRatio < 0.f) newRatio = 0.f;
                        if (newRatio > 1.f) newRatio = 1.f;
                        musiqueActuelle.setPlayingOffset(sf::seconds(newRatio * musiqueActuelle.getDuration().asSeconds()));
                    }
                }
            }
            
        }

        // --- DESSIN DE L'INTERFACE ---
        window.clear(sf::Color(30, 30, 30)); // Fond gris foncé

        // Affichage du titre
        sf::Text txtTitre(font, "NERDCORE SOUNDBOARD", 24); // En SFML 3, la font se passe en PREMIER paramètre
        txtTitre.setFillColor(sf::Color::Cyan);
        txtTitre.setPosition({160.f, 20.f}); // SFML 3 utilise des structures sf::Vector2f pour les positions
        window.draw(txtTitre);

        // Affichage du statut actuel
        sf::Text txtStatut(font, "En cours : " + chansonEnCours, 16);
        txtStatut.setFillColor(sf::Color::White);
        txtStatut.setPosition({40.f, 70.f});
        window.draw(txtStatut);

        // Dessin des boutons dynamiques pour la Playlist
        for (size_t i = 0; i < listeBoutons.size(); ++i) {
            // Rectangle du bouton
            window.draw(listeBoutons[i]);

            window.draw(listeTextes[i]);
        }
        // Dessin du rectangle de duration (barre de progression)
        window.draw(barreProgression);
        
        if (musiqueActuelle.getStatus() == sf::SoundSource::Status::Playing) {
            // Calcul du ratio de progression
            ratio = musiqueActuelle.getPlayingOffset().asSeconds() / musiqueActuelle.getDuration().asSeconds();
        }else if (chansonEnCours == "Aucune") {
                ratio = 0.0f;
        }

        if ((musiqueActuelle.getDuration().asMilliseconds() - musiqueActuelle.getPlayingOffset().asMilliseconds()) <= 20) {
            for (size_t i = 0; i < listeMusiques.size(); ++i) {
                if (chansonEnCours == listeMusiques[i]) {
                    musiqueActuelle.stop();
                    if (i + 1 < listeMusiques.size()) {
                        if (musiqueActuelle.openFromFile("musique/" + listeMusiques[i + 1])) {
                            musiqueActuelle.play();
                            chansonEnCours = listeMusiques[i + 1];
                        }
                    } else {
                        if (musiqueActuelle.openFromFile("musique/" + listeMusiques[0])) {
                            musiqueActuelle.play();
                            chansonEnCours = listeMusiques[0];
                        }
                    }
                    break;
                }
            }
        }
        sf::RectangleShape barreRemplie(sf::Vector2f(400.f * ratio, 20.f));
        barreRemplie.setPosition({100.f, 380.f});
        window.draw(barreRemplie);

        
        window.draw(btnPrevious);

        sf::Text txtPrevious(font, "PREVIOUS", 14);
        txtPrevious.setPosition({30.f, 430.f});
        window.draw(txtPrevious);

        // Dessin du bouton permanent PAUSE
        sf::RectangleShape btnPause(sf::Vector2f(130.f, 40.f));
        btnPause.setPosition({150.f, 420.f});
        btnPause.setFillColor(sf::Color(100, 70, 20));
        window.draw(btnPause);
        
        std::string pauseTexteBouton = "PAUSE";
        if (musiqueActuelle.getStatus() == sf::SoundSource::Status::Playing){
            pauseTexteBouton = "PAUSE";
        }else{
            pauseTexteBouton = "PLAY";
        }
        sf::Text txtPause(font, pauseTexteBouton, 14);
        txtPause.setPosition({170.f, 430.f});
        window.draw(txtPause);

        
        window.draw(btnStop);

        sf::Text txtStop(font, "STOP", 14);
        txtStop.setPosition({365.f, 430.f});
        window.draw(txtStop);

        window.draw(btnNext);

        sf::Text txtNext(font, "NEXT", 14);
        txtNext.setPosition({525.f, 430.f});
        window.draw(txtNext);

        window.display();
    }

    return 0;
}
