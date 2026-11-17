# EVE – Experiment 001
## Entwicklung eines selbstorganisierenden neuronalen Netzwerks

**Stand:** 17. November 2026  
**Status:** Konzept- und Übergabespezifikation  
**Projekt:** EVE – Emergent Virtual Evolution  
**Repository:** `Dntouch/EVE`  
**Implementierung:** C/C++  
**Zielplattform:** Zunächst PC, langfristig auch Mikrocontroller

---

# 1. Zielsetzung

EVE soll ein biologisch inspiriertes, eigenständig lernendes neuronales System entwickeln.

Der neuronale Kern soll ohne vortrainiertes Large Language Model, ohne fest zugewiesene semantische Bedeutungen und ohne vorprogrammierte Wissensstrukturen funktionieren.

Experiment 001 untersucht die Entstehung und Entwicklung eines neuronalen Netzwerks aus einfachen Neuronen, gerichteten Synapsen und angeborenen Entwicklungsregeln.

Dabei sollen folgende Eigenschaften untersucht werden:

- Entstehung einer neuronalen Grundvernetzung.
- Verarbeitung und Weiterleitung binärer Signale.
- Selbstständige Verstärkung synaptischer Eingänge.
- Bildung zusätzlicher Verbindungen.
- Entwicklung wiederkehrender Aktivitätsmuster.
- Speicherung und Wiederaktivierung erlernter Zustände.
- Stabilität und Ressourcenverbrauch des Netzwerks.

Das Experiment dient ausdrücklich der Untersuchung der zugrunde liegenden Mechanismen. Die konkrete Netzwerkgröße und die verwendeten Zahlenwerte sind experimentelle Ausgangsparameter.

# 2. Übergeordnete Architektur

Das System wird funktional in drei Bereiche unterteilt.

## 2.1 BIOS

Das BIOS stellt die technische Ausführungsumgebung bereit.

Aufgaben:

- Speicherverwaltung.
- Ereignisverarbeitung.
- Initialisierung und Persistenz.
- Bereitstellung technischer Eingangs- und Ausgangsschnittstellen.
- Reproduzierbare Zufallszahlen.
- Ressourcenbegrenzung und Messung.
- Diagnose und Protokollierung.

Das BIOS enthält keine sprachlichen Bedeutungen, Erinnerungen oder fertigen Entscheidungsregeln.

## 2.2 Stammhirn

Das Stammhirn enthält die angeborenen Entwicklungs- und Lernmechanismen.

Dazu gehören:

- Regeln zur Bildung neuronaler Verbindungen.
- Regeln zur Veränderung synaptischer Gewichtungen.
- Steuerung lokaler Wachstumsversuche.
- Grundlegende neuronale Regulation.
- Verarbeitung neuronaler Aktivitätsphasen.
- Spätere Mechanismen für Abschwächung und Synapsenschwund.

Das Stammhirn legt nicht fest, welche Neuronen bestimmte Informationen repräsentieren.

Es definiert die Voraussetzungen, unter denen sich das Netzwerk entwickeln kann.

## 2.3 Neuronales Netzwerk

Das neuronale Netzwerk besteht aus:

- Neuronen.
- Gerichteten Synapsen.
- Eingangsgewichtungen.
- Internen neuronalen Zuständen.
- Dynamisch entstehenden Verbindungen.
- Erlernten Aktivitäts- und Verbindungsmustern.

Es erhält keine vorgegebenen semantischen Zuordnungen.

Ein einzelnes Neuron kann an beliebig vielen unterschiedlichen Mustern beteiligt sein.

**Bedeutung entsteht durch das Zusammenspiel neuronaler Zustände, Verbindungen und Aktivitätsmuster.**

# 3. Grundmodell des Neurons

## 3.1 Eigenschaften

Jedes Neuron besitzt mindestens:

- Eine eindeutige numerische ID.
- Ein Membranpotenzial.
- Eine Erregungsschwelle.
- Einen lokalen Eingangszähler.
- Einen einzigen logischen Ausgang.
- Eine Menge eingehender Synapsen.
- Eine Menge ausgehender Synapsen beziehungsweise deren effiziente Referenzierung.

Zusätzliche Zustände werden nur eingeführt, wenn sie für einen definierten Mechanismus erforderlich sind.

## 3.2 Signalübertragung

Die kleinste übertragene Information ist ein binärer Impuls.

- `0`: Kein Impuls.
- `1`: Impuls.

Ein Neuron überträgt beim Feuern ein Ereignis über seinen einzigen logischen Ausgang.

Alle angeschlossenen ausgehenden Synapsen erhalten dasselbe Ereignis.

Der Sender nimmt keine individuelle Gewichtung vor.

## 3.3 Signalempfang

Beim Empfang eines Impulses:

1. Das empfangende Neuron identifiziert die eingehende Synapse.
2. Der lokale Neuronenzähler wird erhöht.
3. Der Aktivitätszähler der betreffenden Synapse wird erhöht.
4. Die synaptische Gewichtung bestimmt die Wirkung auf das Membranpotenzial.
5. Das Neuron prüft seine Erregungsschwelle.
6. Bei Erreichen der Schwelle feuert das Neuron.

Die genaue Behandlung des Membranpotenzials nach dem Feuern bleibt ein zu definierender Implementierungsparameter.

Die Lernzähler arbeiten unabhängig davon, ob das Neuron selbst feuert.

# 4. Synapsenmodell

## 4.1 Grundregeln

Synapsen sind gerichtete Verbindungen.

Für Experiment 001 gilt:

- Maximal eine gerichtete Synapse von A nach B.
- Eine zusätzliche Verbindung von B nach A ist unabhängig davon möglich.
- Selbstverbindungen werden zunächst ausgeschlossen.
- Eine Synapse besitzt einen Sender und einen Empfänger.
- Die synaptische Gewichtung wird logisch beim Empfänger verwaltet.

## 4.2 Gewichtete Eingänge

Ein Neuron verwaltet seine Eingangsgewichtungen selbst.

Synapsen mit identischer Gewichtung können gemeinsame Gewichtungsgruppen verwenden.

Beispiel:

Neuron B empfängt Signale über 100 Synapsen.

Davon besitzen:

- 50 Synapsen Gewichtung 1.
- 30 Synapsen Gewichtung 2.
- 20 Synapsen Gewichtung 3.

Logisch werden nur drei unterschiedliche Gewichtungswerte benötigt.

Jede Synapse bleibt dennoch individuell einer Gewichtung zugeordnet.

Die physische Speicherorganisation ist von diesem logischen Modell getrennt.

Ob die Gruppierung tatsächlich Speicher spart, muss gemessen werden.

## 4.3 Neue Synapsen

Jede neu entstandene Synapse erhält zunächst die Gewichtung 0.

Sie ist unmittelbar nach ihrer Entstehung funktionsfähig.

Das bedeutet:

- Der Sender kann über sie Impulse übertragen.
- Der Empfänger registriert diese Impulse.
- Die Synapse kann lernen.
- Die Gewichtung 0 bewirkt zunächst keine Veränderung des Membranpotenzials.

Eine Synapse mit Gewichtung 0 ist daher vorhanden, aber noch nicht erregend wirksam.

# 5. Neuronale Aktivitätsphasen

## 5.1 Grundprinzip

Jedes Neuron besitzt einen eigenen Aktivitätszähler.

Dieser zählt sämtliche eingehenden Impulse.

Die Gewichtung der auslösenden Synapse spielt dabei keine Rolle.

Die Aktivitätsphase ist unabhängig vom Feuern des Neurons.

Ein Neuron kann eine vollständige Aktivitätsphase durchlaufen, ohne selbst ein einziges Mal zu feuern.

## 5.2 Experimenteller Startwert

Für Experiment 001 wird vorläufig festgelegt:

**8 eingehende Impulse bilden eine Aktivitätsphase.**

Dieser Wert ist experimentell und kann in späteren Vergleichsversuchen verändert werden.

## 5.3 Ablauf

Bei jedem eingehenden Impuls:

1. Neuronenzähler +1.
2. Synapsenzähler der auslösenden Verbindung +1.
3. Prüfung der synaptischen Lernschwelle.
4. Verarbeitung des gewichteten Impulses.
5. Prüfung der neuronalen Erregungsschwelle.
6. Gegebenenfalls Feuern.
7. Gegebenenfalls Abschluss der Aktivitätsphase.

Beim Abschluss der Aktivitätsphase:

- Alle Synapsenzähler des Neurons werden auf 0 gesetzt.
- Der Neuronenzähler wird auf 0 gesetzt.
- Die nächste Aktivitätsphase beginnt mit dem nächsten Eingangssignal.

Eine Aktivitätsphase endet ausschließlich durch das Erreichen des lokalen Ereignislimits.

Es werden keine Zeitstempel pro Synapse benötigt.

# 6. Synaptische Verstärkung

## 6.1 Grundregel

Wiederholt eintreffende Impulse können die Gewichtung einer Synapse erhöhen.

Dafür ist kein Feuerereignis des empfangenden Neurons erforderlich.

## 6.2 Experimentelle Lernschwelle

Als Startwert wird festgelegt:

**Nach jeweils drei Impulsen über dieselbe Synapse wird ihre Gewichtung um 1 erhöht.**

Anschließend wird ausschließlich der betreffende Synapsenzähler auf 0 zurückgesetzt.

## 6.3 Beispiel

Synapse A → B beginnt mit Gewichtung 0.

| Impuls | Synapsenzähler | Gewicht |
|---|---:|---:|
| 1 | 1 | 0 |
| 2 | 2 | 0 |
| 3 | 0 | 1 |
| 4 | 1 | 1 |
| 5 | 2 | 1 |
| 6 | 0 | 2 |
| 7 | 1 | 2 |

Sieben Impulse innerhalb einer Aktivitätsphase ergeben somit eine Gewichtserhöhung um 2.

## 6.4 Gleichzeitiges Erreichen von Grenzwerten

Wenn ein eingehender Impuls gleichzeitig

- den Synapsenzähler auf seine Lernschwelle und
- den Neuronenzähler auf sein Phasenlimit

bringt, wird zuerst die synaptische Verstärkung durchgeführt.

Erst danach endet die Aktivitätsphase.

Der letzte Impuls wird somit vollständig berücksichtigt.

## 6.5 Grenzen

Die maximal zulässige Gewichtung ist noch nicht endgültig definiert.

Ebenso offen ist, wie das Netzwerk mit einer dauerhaft hohen Eingangsgewichtung umgehen soll.

Für erste Tests müssen diese Grenzen als konfigurierbare Versuchsparameter vorliegen.

# 7. Initiale Netzwerkgröße

## 7.1 Neuronen

Experiment 001 beginnt mit:

**256 Neuronen**

Diese Anzahl dient der überschaubaren Untersuchung und vollständigen Beobachtbarkeit.

## 7.2 Initiale Synapsen

Jedes Neuron erhält zunächst vier zufällig ausgewählte ausgehende Synapsen.

Daraus ergeben sich:

**1.024 gerichtete Synapsen**

Die anfänglichen Verbindungspartner werden reproduzierbar zufällig aus dem gesamten Netzwerk ausgewählt.

Ausgeschlossen sind:

- Selbstverbindungen.
- Doppelte gerichtete Verbindungen zwischen denselben Neuronen.

Diese initiale Vernetzung dient als Startbedingung.

Die spätere Bildung neuer Synapsen folgt den Entwicklungsregeln.

# 8. Dreidimensionale neuronale Umgebung

## 8.1 Grundprinzip

Die Neuronen werden in einem zusammenhängenden Speicherbereich verwaltet.

Dieser Speicherbereich wird bei der Initialisierung einmalig logisch zu einem dreidimensionalen Raum gefaltet.

Die physische Speicherreihenfolge bleibt unverändert.

## 8.2 Startgeometrie

Für 256 Neuronen:

**8 × 8 × 4**

Jedes Neuron besitzt damit eine virtuelle Position im dreidimensionalen Raum.

Die Position ergibt sich aus seiner ID und den Abmessungen des virtuellen Volumens.

Es werden keine individuellen Koordinaten gespeichert.

## 8.3 Eigenschaften

- Die Faltung erfolgt einmalig.
- Die virtuellen Positionen bleiben anschließend fest.
- Neuronen werden nicht dynamisch verschoben.
- Räumliche Nähe kann rechnerisch bestimmt werden.
- Die virtuelle Position besitzt keine semantische Bedeutung.

Die dreidimensionale Umgebung dient insbesondere der Partnersuche beim Synapsenwachstum.

# 9. Entstehung neuer Synapsen

## 9.1 Grundsatz

Synapsen entstehen abhängig von neuronaler Aktivität und internem Zustand.

Es gibt keine automatische Bildung einer Synapse bei jedem abgeschlossenen Aktivitätsfenster.

## 9.2 Voraussetzung

Ein Neuron kann neue ausgehende Synapsen nur bilden, wenn es selbst feuert.

Ein Neuron, das lediglich Eingangssignale empfängt, kann dadurch seine Eingangsgewichtungen verändern.

Es beginnt jedoch keine eigene ausgehende Partnersuche, solange es nicht selbst feuert.

## 9.3 Wachstumswahrscheinlichkeit

Die Wahrscheinlichkeit eines Wachstumsversuchs hängt von der Anzahl bereits vorhandener ausgehender Synapsen ab.

Als experimenteller Kandidat gilt:

`P(Wachstum) = 1 / (1 + k)`

Dabei ist `k` die Anzahl vorhandener ausgehender Synapsen.

Beispiele:

| Ausgehende Synapsen | Wahrscheinlichkeit |
|---|---:|
| 0 | 100 % |
| 1 | 50 % |
| 4 | 20 % |
| 9 | 10 % |
| 19 | 5 % |
| 99 | 1 % |

Die Wahrscheinlichkeit nähert sich mit wachsender Vernetzung der Null an.

Eine feste maximale Synapsenzahl pro Neuron wird nicht vorgegeben.

Die Formel ist ein experimenteller Startkandidat und keine abschließend bestätigte Wachstumsregel.

## 9.4 Partnersuche

Bei einem erfolgreichen Wachstumsversuch sucht das Neuron einen neuen Verbindungspartner.

Dabei gilt:

1. Die Suche beginnt in der unmittelbaren dreidimensionalen Nachbarschaft.
2. Bereits verbundene Zielneuronen werden ausgeschlossen.
3. Selbstverbindungen sind ausgeschlossen.
4. Ist kein geeigneter Partner vorhanden, kann der Suchradius erweitert werden.
5. Neue Verbindungen erhalten Gewichtung 0.

Die konkrete Distanzfunktion, der anfängliche Suchradius und die Auswahl zwischen mehreren Kandidaten sind noch als Versuchsparameter zu definieren.

## 9.5 Rückmeldungen

Eine direkte Rückmeldung des Empfängers an den Sender wird für Experiment 001 nicht vorausgesetzt.

Retrograde Signalmechanismen bleiben eine mögliche spätere Erweiterung.

# 10. Abschwächung und Synapsenschwund

## 10.1 Grundsatz

Synapsen müssen grundsätzlich auch wieder verschwinden können.

Der Abbau von Verbindungen soll ein möglicher Bestandteil neuronalen Lernens und struktureller Anpassung sein.

Er wird nicht ausschließlich als Maßnahme zur Freigabe von Speicher betrachtet.

## 10.2 Entfernungsregel

**Sinkt die Eingangsgewichtung einer Synapse unter 0, wird diese Synapse entfernt.**

Eine Gewichtung von 0 bedeutet dagegen:

- Verbindung vorhanden.
- Signalempfang möglich.
- Lernen weiterhin möglich.
- Keine unmittelbare Wirkung auf das Membranpotenzial.

## 10.3 Noch offene Abschwächungsregel

Bisher wurde kein Mechanismus beschlossen, der die Gewichtung einer Synapse tatsächlich reduziert.

Ausdrücklich nicht vorgesehen ist eine automatische Abschwächung allein aufgrund langer Nichtbenutzung.

Zu untersuchen sind:

- Aktivitätsabhängige Konkurrenz.
- Heterosynaptische Plastizität.
- Synaptische Normalisierung.
- Begrenzte Gewichtsbudgets.
- Überlagerung und Veränderung erlernter Muster.

Eine nicht verwendete Verbindung darf grundsätzlich unbegrenzt bestehen bleiben können.

# 11. Synaptische Gewichtsbudgets

## 11.1 Maximalgewicht

Als mögliche Begrenzung wird ein maximal zulässiger Gewichtungswert pro Synapse untersucht.

Beispielsweise könnte ein Gewicht durch eine begrenzte Anzahl Bits dargestellt werden.

Der konkrete Wertebereich ist noch nicht beschlossen.

## 11.2 Gesamtbudget pro Neuron

Zusätzlich wird untersucht, ob die Summe der positiven Eingangsgewichtungen eines Neurons begrenzt werden sollte.

Bei ausgeschöpftem Budget könnte eine weitere Verstärkung nur erfolgen, wenn andere Verbindungen entsprechend abgeschwächt werden.

Dadurch wäre eine Umverteilung synaptischer Stärke möglich.

Die Auswahl der abzuschwächenden Verbindungen ist noch ungeklärt.

## 11.3 Abhängigkeit von ausgehenden Synapsen

Als zusätzliche Hypothese wurde vorgeschlagen, das Eingangsgewichtsbudget eines Neurons von der Anzahl seiner ausgehenden Synapsen abhängig zu machen.

Beispiel einer möglichen Beziehung:

`Budget = Grundbudget + Faktor × Ausgangsanzahl`

Dieser Ansatz könnte dazu führen, dass stärker vernetzte Neuronen größere Eingangsgewichte unterhalten können.

Gleichzeitig kann dadurch eine positive Rückkopplung entstehen:

Mehr Ausgänge → größeres Eingangsbudget → stärkere Erregung → häufigeres Feuern → weitere Wachstumsgelegenheiten.

Diese Hypothese wird zunächst dokumentiert, aber nicht als verbindlicher Bestandteil von Experiment 001 festgelegt.

# 12. Membranpotenzial und neuronales Feuern

Die Verarbeitung des Membranpotenzials ist von der synaptischen Lernregel getrennt.

Das Neuron integriert die gewichteten Eingangssignale.

Erreicht das Membranpotenzial seine Erregungsschwelle, erzeugt es einen Ausgangsimpuls.

Noch zu definieren sind:

- Anfangswert des Membranpotenzials.
- Erregungsschwelle.
- Verhalten nach einem Feuerereignis.
- Mögliche Erholungsphase.
- Behandlung zeitlich auseinanderliegender Impulse.
- Vermeidung unkontrollierter Rückkopplungen.

Im bestehenden neuronalen Grundkonzept wird ein zeitabhängiges Abklingen des Membranpotenzials bevorzugt.

In der neueren Diskussion wurde zusätzlich eine rein ereignisbasierte Verarbeitung ohne individuelle Zeitstempel betrachtet.

**Diese beiden Ansätze sind noch nicht abschließend miteinander vereinbart.**

Für die Implementierung muss deshalb eine konkrete, austauschbare Regel festgelegt werden.

# 13. Neuronale Zeit

EVE soll möglichst ohne permanente globale Aktualisierungsdurchläufe arbeiten.

Die bisher bevorzugte Grundlage ist die Verarbeitung diskreter Ereignisse.

Neuronale Aktivitätsphasen werden durch die Anzahl eingehender Impulse begrenzt.

Damit entsteht eine lokale Ereigniszeit.

Beispielsweise kann ein Neuron innerhalb kurzer physikalischer Zeit viele Aktivitätsphasen durchlaufen, während ein anderes über lange Zeit keine Phase abschließt.

Die physikalische Zeit und die neuronale Ereigniszeit sind deshalb getrennt zu betrachten.

Zeitstempel pro Synapse werden vermieden.

Für das Membranpotenzial können dennoch gesonderte zeitabhängige Mechanismen erforderlich sein.

# 14. Speicherarchitektur

## 14.1 Grundprinzip

Die Implementierung erfolgt in C/C++.

Neuronen und Synapsen sollen in kompakten, zusammenhängenden Speicherbereichen verwaltet werden.

## 14.2 Neuronen

Neuronen werden über numerische Indizes adressiert.

Es werden keine eigenständigen dynamischen Objekte pro Neuron benötigt.

## 14.3 Synapsen

Synapsen werden als kompakte gerichtete Verbindungen gespeichert.

Die physische Darstellung soll möglichst geringe Verwaltungsdaten benötigen.

Der Sender muss seine ausgehenden Empfänger effizient erreichen können.

Der Empfänger muss die zugehörige Eingangsgewichtung effizient ermitteln können.

## 14.4 Dynamische Gewichtungsgruppen

Identische Eingangsgewichtungen können gemeinsam verwaltet werden.

Die Speicherstruktur muss dennoch individuelle Gewichtsänderungen einzelner Synapsen ermöglichen.

Die tatsächliche Effizienz dieser Gruppierung wird experimentell untersucht.

## 14.5 Zähler

Für die ersten Versuche sind vorgesehen:

- Ein lokaler Ereigniszähler pro Neuron.
- Ein lokaler Aktivitätszähler pro Synapse.

Bei einem Phasenlimit von acht Impulsen benötigt der Neuronenzähler mindestens vier Bit.

Bei einer Lernschwelle von drei Impulsen kann ein Synapsenzähler mit zwei Bit dargestellt werden.

Eine bitgepackte Speicherung ist möglich, aber nicht zwingend Voraussetzung für die erste Referenzimplementierung.

## 14.6 Spätere Optimierungen

Als mögliche Ausbaustufe bleibt eine hybride Verbindungsverwaltung vorgesehen.

Dabei könnten sehr dicht vernetzte Bereiche anders gespeichert werden als dünn vernetzte Bereiche.

Diese Optimierung gehört ausdrücklich nicht zu Experiment 001.

# 15. Experimentelle Durchführung

## 15.1 Initialisierung

Das System erzeugt:

- 256 Neuronen.
- 1.024 initiale gerichtete Synapsen.
- Eine logische 8 × 8 × 4-Geometrie.
- Reproduzierbare Zufallszustände.
- Definierte Anfangswerte der neuronalen Zustände.

## 15.2 Aktivierung

Das Netzwerk erhält kontrollierte Eingangssignale.

Zusätzlich kann spontane Aktivität untersucht werden.

Ein Mechanismus zur spontanen Aktivierung ist noch nicht abschließend definiert.

## 15.3 Lernen

Während der Verarbeitung werden:

- Neuronenzähler erhöht.
- Synapsenzähler erhöht.
- Gewichtungen bei Erreichen der Lernschwelle verstärkt.
- Aktivitätsphasen abgeschlossen.
- Gegebenenfalls Wachstumsversuche ausgelöst.

## 15.4 Netzwerkentwicklung

Bei eigenen Feuerereignissen kann ein Neuron entsprechend seiner Wachstumswahrscheinlichkeit versuchen, eine neue ausgehende Synapse zu bilden.

Neue Kontakte werden bevorzugt in seiner dreidimensionalen Umgebung gesucht.

## 15.5 Beobachtung

Zu erfassen sind insbesondere:

- Anzahl aktiver Neuronen.
- Anzahl Feuerereignisse.
- Anzahl vorhandener Synapsen.
- Anzahl neu entstandener Synapsen.
- Verteilung synaptischer Gewichtungen.
- Anzahl abgeschlossener Aktivitätsphasen.
- Wiederkehrende Aktivitätsmuster.
- Speicherverbrauch.
- Rechenaufwand.
- Reproduzierbarkeit.

# 16. Experimentelle Kontrollen

Mindestens folgende Varianten sollen vergleichbar untersucht werden:

**Variante A – Statisches Netzwerk**

Keine synaptische Verstärkung und kein Wachstum.

**Variante B – Synaptisches Lernen**

Gewichtsverstärkung ist aktiv, Synapsenwachstum deaktiviert.

**Variante C – Lernen und Wachstum**

Gewichtsverstärkung und aktivitätsabhängiges Synapsenwachstum sind aktiv.

Spätere Varianten können zusätzliche Abschwächungs- und Normalisierungsmechanismen untersuchen.

Alle Vergleichsläufe müssen mit reproduzierbaren Ausgangsbedingungen durchgeführt werden können.

# 17. Erfolgskriterien

Das Experiment soll messbar beantworten:

1. Entstehen reproduzierbare neuronale Aktivitätsmuster?
2. Verändern wiederholte Eingangsereignisse die Gewichtungen?
3. Führt die Gewichtsveränderung zu verändertem neuronalen Verhalten?
4. Können bereits entstandene Aktivitätsmuster später erneut ausgelöst werden?
5. Wie entwickelt sich die Anzahl der Synapsen?
6. Wie verändert sich die Vernetzungsdichte?
7. Entstehen lokal oder global besonders stark vernetzte Bereiche?
8. Kommt es zu unkontrollierter Aktivierung?
9. Kommt die Netzwerkaktivität zum Erliegen?
10. Welche Auswirkungen haben die gewählten Entwicklungsparameter?
11. Wie viel Speicher und Rechenleistung werden benötigt?
12. Bleiben erlernte Zustände nach Speicherung und Wiederherstellung erhalten?

Die Messergebnisse müssen nachvollziehbar protokolliert werden.

# 18. Offene Entscheidungen vor der Implementierung

Folgende Punkte müssen noch konkretisiert werden:

1. Numerische Darstellung von Membranpotenzial und Gewichtungen.
2. Erregungsschwelle und Verhalten nach dem Feuern.
3. Ereignisbasierte oder zeitabhängige Veränderung des Membranpotenzials.
4. Anfangsgewichtungen der initialen Synapsen.
5. Konkreter räumlicher Suchradius und Erweiterungsregel.
6. Auswahl eines Verbindungspartners innerhalb des Suchradius.
7. Exakte Auslösung und Abwicklung eines Wachstumsversuchs.
8. Mechanismus für spontane Aktivität.
9. Behandlung gleichzeitig oder unmittelbar aufeinanderfolgend ausgelöster Ereignisse.
10. Maximale Ereignisverarbeitung zur Vermeidung endloser Aktivierungsschleifen.
11. Mechanismus zur Abschwächung synaptischer Gewichte.
12. Mögliche Normalisierung und Gewichtsbudgets.
13. Persistenzformat und Wiederherstellung des neuronalen Zustands.

Nicht alle diese Fragen müssen vor einem ersten technischen Funktionstest endgültig beantwortet sein.

Für offene Mechanismen sind ausdrücklich dokumentierte Versuchsvarianten zulässig.

# 19. Anforderungen an die Umsetzung

Die Implementierung soll:

- Auf einem gewöhnlichen PC ausführbar sein.
- Langfristig auf ressourcenbeschränkte Hardware portierbar bleiben.
- Ohne GPU auskommen können.
- Keine vortrainierten neuronalen Modelle benötigen.
- Ohne Python-Laufzeit für den neuronalen Kern funktionieren.
- Reproduzierbare Experimente ermöglichen.
- Zustände speichern und wiederherstellen können.
- Einzelne Lern- und Entwicklungsmechanismen separat aktivieren oder deaktivieren können.
- Speicherverbrauch und Ereignisverarbeitung messbar machen.
- Änderungen am neuronalen Verhalten nachvollziehbar protokollieren.

Der neuronale Kern soll möglichst unabhängig von Betriebssystem, Benutzeroberfläche und Kommunikationsschnittstellen bleiben.

# 20. Abgleich mit bestehendem Git-Projekt

Vor einer Umsetzung sind insbesondere folgende Dokumente zu berücksichtigen:

- `README.md`
- `docs/konzept/EVE-Konzept.md`
- `docs/konzept/EVE-Neuronales-Grundkonzept.md`
- `docs/konzept/EVE-Dynamische-gewichtete-Neuroneneingaenge.md`
- `docs/ROADMAP.md`
- `docs/FORSCHUNGSFRAGEN.md`
- `docs/entscheidungen/0002-technologie-stack-bleibt-offen.md`

Die bisherige Dokumentation enthält noch ältere oder allgemeinere Architekturannahmen.

Insbesondere sind die folgenden neueren Festlegungen abzugleichen:

- C/C++ als gewählter Implementierungsweg.
- Gewichtete Eingänge statt gewichteter Ausgänge.
- Einmalig gefalteter dreidimensionaler Speicherraum.
- Lokale neuronale Aktivitätsphasen.
- Unabhängige synaptische Lernzähler.
- Aktivitätsabhängiges Wachstum ausgehender Synapsen.
- Gewichtung 0 als Ausgangszustand neuer Verbindungen.
- Synapsenentfernung bei Gewichtung unter 0.

Bestehende Entscheidungen sollen nicht stillschweigend überschrieben werden.

Neue Architekturentscheidungen sind gemäß dem vorhandenen Entscheidungsprozess zu dokumentieren.

# 21. Nächster Arbeitsschritt

Nach Übernahme und Abgleich der Spezifikation soll Experiment 001 als reproduzierbarer C/C++-Versuch vorbereitet werden.

Zunächst sollen die bereits festgelegten Mechanismen implementiert und isoliert überprüft werden.

Noch ungeklärte Lernmechanismen dürfen nicht durch undokumentierte Annahmen ersetzt werden.

Insbesondere bleiben Abschwächung, Vergessen und synaptische Normalisierung eigenständige Forschungsfragen.

---

## Abschließender Grundsatz

**Wir programmieren nicht, welche Informationen EVE lernen soll. Wir definieren die Bedingungen, unter denen ihr neuronales Netzwerk Erfahrungen sammeln, sich verändern und daraus eigene Strukturen entwickeln kann.**

Die Ergebnisse dieser Entwicklung werden experimentell untersucht und nicht vorweggenommen.