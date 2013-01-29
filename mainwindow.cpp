#include "mainwindow.h"


void mainWindow::displayChutier()
{
    chutier = new Chutier(this);
    chutier->setStyleSheet("border : 1px solid #d9d9d9;"
                           "border-top-left-radius:5px;"
                           "border-top-right-radius:5px;"
                           "padding-top:20px;");
    chutier->resize(300,600);

    HSplitter->addWidget(chutier);

    chutier->show();
}

void mainWindow::displayPinceau()
{
    pinceau = new Pinceau(this);
    pinceau->setStyleSheet("border : 1px solid #d9d9d9;"
                           "border-top-left-radius:5px;"
                           "border-top-right-radius:5px;"
                           "padding-top:20px;");

    subRightSplitter->addWidget(pinceau);
    pinceau->show();
}

void mainWindow::displayMoniteur()
{
    moniteur = new Moniteur(this);
    moniteur->setStyleSheet("border-top-left-radius:5px;"
                            "border-top-right-radius:5px;");
    subRightSplitter->addWidget(moniteur);
    moniteur->show();
}

void mainWindow::displayTimeline()
{
    timeline = new Timeline(this);
    timeline->resize(900,400);

    timeline->setStyleSheet("border : 1px solid #d9d9d9;"
                            "border-top-left-radius:5px;"
                            "border-top-right-radius:5px;"
                            "padding-top:15px;");

    VSplitter->addWidget(timeline);
    timeline->show();
}

void mainWindow::displayExportWindow()
{
    ui_exportWindow = new exportWindow();
    ui_exportWindow->setModal(true); //on empeche le clic sur le reste de la fenetre
    ui_exportWindow->exec();

    delete ui_exportWindow;
    ui_exportWindow = NULL;
}

mainWindow::mainWindow()
{
    centralWidget = new QWidget(this);
    QWidget *container = new QWidget(this); //widgets moniteur + pinceau + timeline

    mainLayout = new QHBoxLayout;

    VSplitter = new QSplitter(Qt::Vertical, this);
    HSplitter = new QSplitter(Qt::Horizontal, this);

    initMenu();

    displayChutier();

    rightVLayout = new QVBoxLayout;

    QWidget *subContainer = new QWidget(this); //pinceau + moniteur
    hLayout = new QHBoxLayout;
    subRightSplitter = new QSplitter(Qt::Horizontal, this);

    displayPinceau();
    displayMoniteur();
    VSplitter->addWidget(subContainer);

    hLayout->addWidget(subRightSplitter);

    subContainer->setLayout(hLayout);
/*
    container->setStyleSheet("border:1px solid blue;");
    VSplitter->setStyleSheet("border : 1px solid red;");
    subContainer->setStyleSheet("border : 1px solid black;");
*/
    displayTimeline();

    rightVLayout->addWidget(VSplitter);

    container->setLayout(rightVLayout);

    HSplitter->addWidget(container);
    mainLayout->addWidget(HSplitter);
    HSplitter->setStretchFactor(1,2);


    connect(timeline,SIGNAL(sig_show(bool)), this, SLOT(updateTimelineAction(bool)));
    connect(pinceau,SIGNAL(sig_show(bool)), this, SLOT(updatePinceauAction(bool)));
    connect(moniteur,SIGNAL(sig_show(bool)), this, SLOT(updateMoniteurAction(bool)));
    connect(chutier,SIGNAL(sig_show(bool)), this, SLOT(updateChutierAction(bool)));

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

}

mainWindow::~mainWindow()
{

}

void mainWindow::newProject()
{
    std::cout<<"New Project"<<std::endl;
}

void mainWindow::openProject()
{
    QString projectname = QFileDialog::getOpenFileName(this, "Ouvrir le fichier", "/home/");

    //AJOUTER LES FILTRES DE RESTRICTION

    if(projectname.isEmpty())
    {
        std::cout << "Aucun fichier n'a été sélectionné" << std::endl;
    }
}

void mainWindow::save()
{
    //lors de l'ouverture d'un projet, enregistrer le dossier de travail dans une variable.
    //si variable vide, rediriger vers Enregistrer sous...
    std::cout << "enregistrer" << std::endl;
}

void mainWindow::saveUnder()
{
    QString projectname = QFileDialog::getSaveFileName(this,tr("Enregistrer sous..."),"/home");
}

void mainWindow::openFile()
{
    //Charger une vidéo
    QStringList filters;
    filters << "Video files (*.mp4 *.avi *.mov)";

    QFileDialog dialog(this);
    dialog.setNameFilters(filters);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.exec();


    //afficher le nom de la vidéo et son type
    QString fileName = dialog.selectedFiles().first();
    fileName.remove(0, ( fileName.lastIndexOf("/")+1) );

    QList<QString> list;
    list.append(fileName);
    chutier->Add(list);

    //afficher un lien vers le moniteur +lecture accélérée ou retour en arrière
/*
    QIcon button = QIcon("/ressources/lecteur.jpeg");

    QList<QIcon> listP;
    listP.append(button);*/
   // chutier->Play(listP);

    //afficher le poid de la vidéo
    QFileInfo fileWeight(dialog.selectedFiles().first());
    qint64 size = 0;
    size = fileWeight.size()/1000000.;
    QString Size = QString::number(size, 10);

    QList<QString> listW;
    listW.append(Size);
    if(!listW.isEmpty())
        chutier->Weight(listW);

    /*int i = 0;
    bool select;
    for(i=0;i<list.size();++i){
        if(list[i].select=true){
            listW[i].select;
        }
    }*/
}

void mainWindow::showChutier()
{
    if(m_afficherChutier->isChecked()){
       chutier->setVisible(true);
    }
    else{
        chutier->setVisible(false);
    }
}

void mainWindow::showOutils()
{
    if(m_afficherOutils->isChecked()){
        pinceau->setVisible(true);
    }
    else{
        pinceau->setVisible(false);
    }
}

void mainWindow::showMoniteur()
{
    if(m_afficherMoniteur->isChecked()){
        moniteur->setVisible(true);
    }
    else{
        moniteur->setVisible(false);
    }
}

void mainWindow::showTimeline()
{
    if(m_afficherTimeline->isChecked()){
        timeline->setVisible(true);
    }
    else{
        timeline->setVisible(false);
    }
}

void mainWindow::showConception()
{
    ui_aboutConception = new infoConception();
    ui_aboutConception->setModal(true); //on empeche le clic sur le reste de la fenetre
    ui_aboutConception->exec();

    delete ui_aboutConception;
    ui_aboutConception = NULL;

}

void mainWindow::initMenu()
{
    /*------  INSTANCIATION ACTIONS  ------*/

    m_Nouveau = new QAction("Nouveau...", this);
    m_Nouveau->setShortcut(QKeySequence::New);
    m_Ouvrir = new QAction("Ouvrir...", this);
    m_Ouvrir->setShortcut(QKeySequence::Open);
    m_Enregistrer = new QAction("Enregistrer",this);
    m_Enregistrer->setShortcut(QKeySequence::Save);
    m_EnregistrerSous = new QAction("Enregistrer sous...",this);
    m_EnregistrerSous->setShortcut(QKeySequence::SaveAs);
    m_Importer = new QAction("Importer...",this);
    m_Exporter = new QAction("Exporter...", this);
    m_Quitter = new QAction("Quitter", this);
    m_Quitter->setShortcut(QKeySequence::Quit);


    m_appliquerScript = new QAction("Appliquer un script...", this);
    m_supprimerScript = new QAction("Supprimer les scripts appliqués", this);

    m_lireDernierClip = new QAction("Lire le dernier clip importé", this);
    m_lancerRendu = new QAction("Lancer le rendu", this);

    m_afficherChutier = new QAction("Chutier", this);
    m_afficherChutier->setCheckable(true);
    m_afficherChutier->setChecked(true);
    m_afficherOutils = new QAction("Outil", this);
    m_afficherOutils->setCheckable(true);
    m_afficherOutils->setChecked(true);
    m_afficherMoniteur = new QAction("Moniteur", this);
    m_afficherMoniteur->setCheckable(true);
    m_afficherMoniteur->setChecked(true);
    m_afficherTimeline = new QAction("Timeline", this);
    m_afficherTimeline->setCheckable(true);
    m_afficherTimeline->setChecked(true);

    m_afficherConception = new QAction("Conception",this);
    m_afficherRealisation = new QAction("Réalisation",this);
    m_voirSite = new QAction("http://electronicwallpaper.com",this);

    /*------  INSTANCIATION ITEMS MENU  ------*/

    QMenu *fichier = new QMenu("&Fichier");
    QMenu *script = new QMenu("&Script");
    QMenu *lecteur = new QMenu("&Lecteur");
    QMenu *process = new QMenu("&Process");
    QMenu *affichage = new QMenu("&Affichage");
    QMenu *credits = new QMenu("&Crédits");

    QMenuBar *menu = menuBar();

    menu->addMenu(fichier);
    menu->addMenu(script);
    menu->addMenu(lecteur);
    menu->addMenu(process);
    menu->addMenu(affichage);
    menu->addMenu(credits);

    fichier->addAction(m_Nouveau);
    fichier->addAction(m_Ouvrir);
    fichier->addSeparator();
    fichier->addAction(m_Enregistrer);
    fichier->addAction(m_EnregistrerSous);
    fichier->addSeparator();
    fichier->addAction(m_Importer);
    fichier->addAction(m_Exporter);
    fichier->addSeparator();
    fichier->addAction(m_Quitter);

    script->addAction(m_appliquerScript);
    script->addAction(m_supprimerScript);

    lecteur->addAction(m_lireDernierClip);
    process->addAction(m_lancerRendu);

    affichage->addAction(m_afficherChutier);
    affichage->addAction(m_afficherOutils);
    affichage->addAction(m_afficherMoniteur);
    affichage->addAction(m_afficherTimeline);

    credits->addAction(m_afficherConception);
    credits->addAction(m_afficherRealisation);
    credits->addSeparator();
    credits->addAction(m_voirSite);

    /*------  CONNEXIONS ACTIONS  ------*/

    connect(m_Nouveau, SIGNAL(triggered()), this, SLOT(newProject()));
    connect(m_Ouvrir, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(m_Enregistrer, SIGNAL(triggered()), this, SLOT(save()));
    connect(m_EnregistrerSous, SIGNAL(triggered()), this, SLOT(saveUnder()));
    connect(m_Importer, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(m_Exporter, SIGNAL(triggered()), this, SLOT(displayExportWindow()));
    connect(m_Quitter,SIGNAL(triggered()),qApp, SLOT(quit()));

    /*
    connect(m_appliquerScript, SIGNAL(triggered()), this, SLOT());
    connect(m_supprimerScript, SIGNAL(triggered()), this, SLOT());
*/

    connect(m_afficherChutier,SIGNAL(triggered()),this, SLOT(showChutier()));
    connect(m_afficherOutils,SIGNAL(triggered()),this, SLOT(showOutils()));
    connect(m_afficherMoniteur,SIGNAL(triggered()),this, SLOT(showMoniteur()));
    connect(m_afficherTimeline,SIGNAL(triggered()),this, SLOT(showTimeline()));

    /*connect(m_lireDernierClip, SIGNAL(triggered()), this, SLOT());
    connect(m_lancerRendu, SIGNAL(triggered()), this, SLOT());*/

    connect(m_afficherConception, SIGNAL(triggered()), this, SLOT(showConception()));
  /*    connect(m_afficherRealisation, SIGNAL(triggered()), this, SLOT());*/
    connect(m_voirSite, SIGNAL(triggered()), this, SLOT(visitWebsite()));

}

void mainWindow::updateTimelineAction(bool a)
{
    m_afficherTimeline->setChecked(a);
}

void mainWindow::updatePinceauAction(bool a)
{
    m_afficherOutils->setChecked(a);
}

void mainWindow::updateMoniteurAction(bool a)
{
    m_afficherMoniteur->setChecked(a);
}

void mainWindow::updateChutierAction(bool a)
{
    m_afficherChutier->setChecked(a);
}

void mainWindow::visitWebsite()
{
    QDesktopServices::openUrl(QUrl("http://i.imgur.com/A81gtCp.gif"));
}



