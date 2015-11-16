#ifndef TRAVERSER
#define TRAVERSER

#endif // TRAVERSER
#include "mainwindow.h"
#include <QApplication>
#include <fstream>
#include <iostream>
#include <QDebug>
#include <QProcess>
class Traverser:public QObject{
    Q_OBJECT
public:

    QList<QString> privateList;
    MainWindow* w;
    void initTraversing(QString s,MainWindow* w0){
        w=w0;        
        w->stack.push(s);
    }
signals:
    void listReady();
public slots:

    void continueTraversing(){
        std::ofstream fout;
        fout.open("log_scanner.txt", std::ios::app);
            while (w->stack.size()!=0&&privateList.length()<100){
                QString st =  w->stack.top();
                fout <<"Scanning directory: ";
                fout <<st.toStdString()<< std::endl;

                qDebug()<<st;
                 w->stack.pop();
                QDir start(st);
                start.setNameFilters(QStringList() << "*");
                QString currentString;
                if (!start.exists())
                {
                   fout <<"error"<< std::endl;
                }
                QStringList  dir_list = start.entryList(QDir::AllDirs);
                //qDebug()<<dir_list;
                for (int i = 0; i < dir_list.size(); ++i){
                    if (dir_list.at(i) != "."&&dir_list.at(i) != ".."){
                        currentString = st;
                        currentString.append("\\");
                        currentString.append(dir_list.at(i));
                         w->stack.push(currentString);
                    }
                }

                QStringList file_list = start.entryList(QDir::Files);
                for (int i = 0; i < file_list.size(); ++i){
                    currentString = st;
                    currentString.append("\\");
                    currentString.append(file_list.at(i));
                    privateList.append(currentString);

                }

                fout.close();
            }
        if(w->stack.size()==0)
            w->traverserFinished=true;



    }

    void giveMore(){
        privateList.clear();
        if(!w->traverserFinished)continueTraversing();
        w->filesToScan=QList<QString>(privateList);
        emit listReady();

    }
};