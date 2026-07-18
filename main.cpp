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

    if (std::filesystem::exists("musique")) {
        for (const auto& entry : std::filesystem::directory_iterator("musique")) {
            if (entry.path().extension() == ".ogg" || entry.path().extension() == ".wav") {
                listeMusiques.push_back(entry.path().filename().string());
            }
        }
    }

    for (size_t i = 0; i < listeMusiques.size(); ++i) {
        sf::RectangleShape bouton(sf::Vector2f(400.f, 35.f));
        bouton.setPosition({100.f, 100.f + i * 50.f});
        bouton.setFillColor(sf::Color(50, 50, 80));
        bouton.setOutlineThickness(2.f);
        bouton.setOutlineColor(sf::Color::Blue);
        
        // On ajoute ce bouton dans notre liste
        listeBoutons.push_back(bouton);
    }

    

    sf::Music musiqueActuelle;
    std::string chansonEnCours = "Aucune";

    // 4. Boucle principale de la fenêtre
        // 4. Boucle principale de la fenêtre (Version SFML 3)
        
    // Dessin du bouton permanent STOP
    sf::RectangleShape btnStop(sf::Vector2f(130.f, 40.f));
    btnStop.setPosition({320.f, 420.f});
    btnStop.setFillColor(sf::Color(120, 20, 20));

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

            // Texte sur le bouton (nom de la musique)
            sf::Text txtMusique(font, listeMusiques[i], 14);
            txtMusique.setFillColor(sf::Color::White);
            txtMusique.setPosition({110.f, 108.f + i * 50.f});
            window.draw(txtMusique);
        }

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

        window.display();
    }

    return 0;
}
