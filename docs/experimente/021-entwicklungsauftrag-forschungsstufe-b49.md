# Entwicklungsauftrag 021 – Forschungsstufe B.4.9

## Lokale Fehlerkorrektur und Aktivierung konkurrierender Handlungswege

**Projekt:** EVE – Emergent Virtual Evolution  
**Ausgangsbasis:** `codex/research-stage-b48-adaptive-exploration`  
**Technologie:** C++17  
**Forschungsstatus:** Kausale Ursachenforschung mit bedingter neuronaler Erweiterung

---

## 1. Ausgangslage

EVE besitzt inzwischen:

- Ereignisgesteuerte Neuronen mit binären Feuerereignissen.
- Gewichtete erregende und hemmende Synapsen.
- Lokales Ereignisgedächtnis G3.
- Zeitlich begrenzte Eligibility-Spuren.
- Drei-Faktor-Plastizität.
- Geschützte angeborene Exploration.
- Lokal wirksame inhibitorische Konkurrenz.
- Persistente neuronale Zustände.

B.4.8 hat nachgewiesen, dass autonome neuronale Reizbindungen bereits nach 4–8 Erfahrungen entstehen können.

Diese Bindungen bleiben jedoch instabil. Weitere Erfahrungen führen nicht automatisch zu einer Verbesserung.

Die bisherigen Untersuchungen ergaben außerdem:

1. Bereits erworbene, aber falsche autonome Antworten verhindern häufig weitere Exploration.
2. Negative Rückmeldung schwächt beteiligte Synapsen, aktiviert aber nicht zuverlässig eine alternative Handlung.
3. Beide motorischen Wege bleiben strukturell erreichbar.
4. Zusätzliche zufällige Exploration führt nicht zuverlässig zu besserem autonomen Lernen.
5. Das Einfrieren früher synaptischer Gewichte verschlechtert die spätere Anpassungsfähigkeit.
6. Die tatsächliche Lernleistung hängt erheblich von frühen Erfahrungen ab.

### Korrigierte B.4.8-Referenz

| Aufgabe | Autonomer Erfolg E1 |
|---|---:|
| Normale Zielzuordnung, Stromfamilie 0 | 43,82 % |
| Inverse Zielzuordnung, Stromfamilie 0 | 50,14 % |

Andere unabhängige Stromfamilien zeigen vergleichbar unzureichende Ergebnisse.

Eine minimale rückmeldungsabhängige Zusatzexploration konnte diese Situation nicht zuverlässig verbessern.

**B.4.8 hat damit keinen neuen allgemeinen Explorationsmechanismus freigegeben.**

---

## 2. Zentrale Forschungsfrage

**Kann EVE nach einer negativ bewerteten autonomen Entscheidung lokal ihre neuronale Aktivität so verändern, dass zuvor unterlegene Handlungswege wieder wirksam werden und durch weitere Erfahrung eigenständig erlernt werden können?**

Es geht ausdrücklich nicht darum, unmittelbar nach einer falschen Entscheidung technisch die jeweils andere Handlung auszuführen.

Das Ziel ist eine neuronale Fähigkeit zur Fehlerkorrektur.

Dafür müssen drei Prozesse unterschieden werden:

1. Die Registrierung einer negativen Erfahrung.
2. Die Veränderung der lokalen neuronalen Konkurrenz.
3. Die erfahrungsabhängige Verstärkung einer gegebenenfalls erfolgreichen Alternative.

Diese Prozesse dürfen nicht als eine fest programmierte Handlungsauswahl zusammengefasst werden.

---

## 3. Forschungshypothesen

### H1 – Dominanz bereits erlernter Verbindungen

Nach einer erfolgreichen frühen Erfahrung erzeugen verstärkte sensorische Verbindungen bevorzugt dieselbe motorische Antwort.

Auch nach negativer Rückmeldung können diese Verbindungen aktiv genug bleiben, um eine erneute identische Entscheidung auszulösen.

### H2 – Funktionale Unterdrückung alternativer Aktivität

Konkurrierende Wege sind vorhanden, erreichen aber infolge erregender oder inhibitorischer Wechselwirkungen nicht die notwendige Aktivierung.

### H3 – Fehlende lokale Reaktion auf negative Erfahrung

Die bisherige Drei-Faktor-Plastizität verändert synaptische Gewichte, erzeugt aber keinen hinreichenden lokalen Zustand, der beim nächsten relevanten Aktivitätsereignis eine alternative Entwicklung begünstigt.

### H4 – Unzureichende Stabilisierung erfolgreicher Alternativen

Selbst wenn eine andere Handlung ausprobiert und positiv bewertet wird, entsteht daraus nicht zuverlässig eine dauerhafte, autonom abrufbare Verbindung.

Alle Hypothesen sind unabhängig zu untersuchen.

Mehrere können gleichzeitig zutreffen.

---

## 4. Phase A – Reproduktion und methodische Absicherung

Vor funktionalen Veränderungen müssen die korrigierten B.4.8-Referenzversuche exakt reproduziert werden.

### A1 – Referenzen

Zu reproduzieren sind:

- Normale und inverse gemischte Lernaufgaben.
- Alle drei unabhängigen Zufallsstromfamilien.
- Die bisherigen No-Learning-Kontrollen.
- Die feste inhibitorische Kontrolle.
- Die B.4.8-Variante mit zusätzlicher Exploration.
- Die Timingkontrolle.
- Die gespeicherten E1-/E2-Ergebnisse.

Die bisherigen 459 Kerntests bleiben erhalten.

Historisch gekoppelte Zufallsströme dürfen nicht erneut als allgemeine Lernreferenz verwendet werden.

### A2 – Unabhängige Evaluation

Ab B.4.9 müssen E1 und E2 aus **getrennten Kopien desselben vollständig trainierten neuronalen Zustands** ermittelt werden.

**E1-Kopie:**

- Keine zusätzliche Exploration.
- Keine weitere Plastizität.
- Nur sensorisch ausgelöste autonome neuronale Antworten.

**E2-Kopie:**

- Bisherige geschützte Exploration nach ausbleibender Antwort.
- Keine weitere Plastizität.
- Identische äußere Reizfolge.

Dadurch ist ausgeschlossen, dass Explorationsereignisse während der Evaluation nachfolgende autonome Antworten beeinflussen.

Beide Kopien müssen vor der Evaluation denselben vollständigen Zustandsfingerabdruck besitzen.

---

## 5. Phase B – Untersuchung einer einzelnen Fehlentscheidung

Die Untersuchung konzentriert sich zunächst auf wiederholte Präsentationen desselben Reizes.

### B1 – Entscheidung vor negativer Rückmeldung

Erfasse für eine autonome Fehlentscheidung:

- Aktivität des sensorischen Neurons.
- Aktivität der beteiligten Entscheidungsneuronen.
- Aktivität der motorischen Ausgänge.
- Membranpotenziale vor und nach dem Feuerereignis.
- Erregende und hemmende Eingangseinflüsse.
- Aktive synaptische Eligibility-Spuren.
- Gewichte der beteiligten Verbindungen.

### B2 – Negative Rückmeldung

Untersuche anschließend:

- Welche lokalen Eligibility-Spuren die negative Modulation erreichen.
- Welche Gewichte tatsächlich verändert werden.
- Welche aktiven Spuren verbraucht werden.
- Welche neuronalen und synaptischen Zustände bestehen bleiben.
- Ob die zuvor dominierende Verbindung weiterhin ausreichend erregend wirkt.
- Ob inhibitorische Verbindungen die konkurrierende Aktivität verhindern.

### B3 – Wiederholung desselben Reizes

Präsentiere denselben Reiz erneut, ohne zwischenzeitlichen technischen Reset.

Erfasse:

- Wird dieselbe Handlung wiederholt?
- Entsteht eine andere autonome Handlung?
- Bleibt die Ausgabe vollständig aus?
- Feuern beide motorischen Wege?
- Welche synaptischen Änderungen erklären das Verhalten?

Die Diagnose darf den äußeren Reiz und die richtige Zielhandlung kennen.

Die neuronale Verarbeitung selbst erhält ausschließlich lokale Ereignisse und allgemeine Bewertungsrückmeldung.

### B4 – Ereigniskette

Rekonstruiere für ausgewählte Seeds die vollständige Abfolge:

**Reiz → neuronale Aktivität → autonome Entscheidung → negative Bewertung → lokale Gewichtsänderung → erneuter Reiz → neue Entscheidung**

Damit soll der genaue Punkt identifiziert werden, an dem die mögliche Fehlerkorrektur ausbleibt.

---

## 6. Phase C – Kausale Prüfung der Konkurrenz

Es ist zu unterscheiden, ob die ausbleibende alternative Handlung durch erregende Dominanz, inhibitorische Hemmung oder die lokale Lernregel verursacht wird.

### C1 – Unveränderter Zustand

Referenz ohne Eingriff.

### C2 – Inhibitorische Wirkung deaktiviert

In einer diagnostischen Zustandskopie wird ausschließlich die inhibitorische Wirkung deaktiviert.

Untersuche, ob dadurch alternative neuronale Aktivitäten wieder erreichbar werden.

### C3 – Erregende Dominanz reduziert

In einer diagnostischen Kopie wird ausschließlich die Stärke der zuvor dominierenden erregenden Verbindung kontrolliert verändert.

Keine permanente Übernahme in die Lernarchitektur.

### C4 – Inhibitorische Gewichte zurückgesetzt

Untersuche den eigenständigen Beitrag zuvor erlernter inhibitorischer Gewichte.

### C5 – Alternative strukturell erreichbar

Prüfe, ob der unterlegene Entscheidungsweg durch einen isolierten, allgemeinen neuronalen Eingangsimpuls grundsätzlich zum Feuern gebracht werden kann.

Diese Prüfung dient ausschließlich der technischen Diagnose.

Sie darf nicht als erfolgreiche autonome Entscheidung oder Lernleistung gewertet werden.

### C6 – Wiederholte negative Bewertung

Untersuche die Entwicklung nach mehreren negativen Erfahrungen mit demselben Reiz.

Entscheidend ist, ob sich neuronale Aktivität tatsächlich verändert oder lediglich synaptische Gewichte zwischen wenigen Zuständen wechseln.

Alle Eingriffe sind ausschließlich auf diagnostische Zustandskopien anzuwenden.

---

## 7. Phase D – Aktivierung konkurrierender Wege

B.4.8 zeigte, dass bloß mehr zufällige Exploration keinen zuverlässigen Lernvorteil erzeugt.

B.4.9 soll deshalb untersuchen, ob **die zeitliche und neuronale Einbindung einer Alternativerprobung** entscheidend ist.

### D1 – Bestehende Exploration

Unveränderte B.4.8-Referenz.

### D2 – Allgemeine Zusatzexploration

Rückmeldungsunabhängige zusätzliche Aktivität mit kontrollierter Ereigniszahl.

### D3 – Ereignisgebundene Exploration

Zusätzliche allgemeine Exploration nach negativer Rückmeldung, wie in B.4.8 untersucht.

### D4 – Lokale Konkurrenzdiagnose

Untersuche, ob die vorherige autonome Aktivität oder inhibitorische Nachwirkung die Entstehung einer weiteren neuronalen Antwort verhindert.

Variiere ausschließlich zu Diagnosezwecken:

- Abstand zur vorherigen Entscheidung.
- Dauer lokaler inhibitorischer Wirkung.
- Refraktärbedingungen.
- Zeitpunkt der Modulation.
- Reihenfolge kausal aufeinanderfolgender neuronaler Ereignisse.

Technisch gleichzeitige Ereignisse müssen weiterhin ordnungsunabhängig verarbeitet werden.

### D5 – Erfolgreiche Alternative

Wenn eine zufällige alternative Handlung auftritt und positiv bewertet wird, untersuche:

- Welche Synapsen Eligibility besitzen.
- Welche Synapsen verstärkt werden.
- Ob daraus beim nächsten gleichen Reiz eine autonome Antwort entsteht.
- Ob diese neue Antwort die ursprüngliche dominante Verbindung überdauert.
- Ob die zweite Reiz-Handlungs-Beziehung erhalten bleibt.

Das System darf nicht technisch erfahren, dass eine Handlung „die Alternative“ zur vorherigen war.

Dieser Begriff dient ausschließlich der externen Diagnose.

---

## 8. Phase E – Bedingte lokale Fehlerkorrektur

**Ein neuer Mechanismus darf erst eingeführt werden, wenn die vorherigen Phasen einen konkreten neuronalen Engpass identifiziert haben.**

Es darf zunächst höchstens eine minimale neue Forschungsvariante entstehen.

### Mögliche Untersuchungsrichtung

Eine negative neuronale Bewertung könnte vorübergehend die lokale Bereitschaft verändern, eine zuvor nicht ausreichend aktive Verbindung erneut wirksam werden zu lassen.

Dafür kommen ausschließlich lokal verfügbare Informationen infrage:

- Allgemeine negative Modulation.
- Gegenwärtige neuronale Aktivität.
- Lokale Membran- oder Erholungszustände.
- Vorhandene synaptische Eligibility.
- Lokale inhibitorische Wechselwirkungen.

Die genaue Realisierung ist offen.

Es darf nicht ohne vorherigen Kausalnachweis eine bestimmte Implementierung festgelegt werden.

### Verbotene Abkürzungen

Unzulässig sind:

- „Wenn A falsch war, wähle B.“
- „Wenn B falsch war, wähle A.“
- Technische Auswahl des anderen Motorneurons.
- Verwendung der extern richtigen Zielhandlung.
- Reiz- oder Handlungs-IDs innerhalb der Lernentscheidung.
- Rücksetzen aller Gewichte nach negativer Bewertung.
- Globale Abfrage der gesamten neuronalen Aktivität.
- Versteckte Winner-take-all-Entscheidungen.
- Automatische Aktivierung des richtigen Pfads.
- Ein festes Zeitprogramm, das eine erfolgreiche neuronale Entscheidung simuliert.

### Biologische Einordnung

Ein möglicher Mechanismus muss als allgemeine neuronale Fähigkeit formuliert sein.

Das Stammhirn darf beispielsweise allgemeine Zustände für lokale neuronale Erregbarkeit, Hemmung oder explorative Bereitschaft bereitstellen.

Welche Verbindung dadurch tatsächlich aktiviert und später gelernt wird, muss weiterhin aus neuronaler Aktivität und Erfahrung entstehen.

### Implementierungsort und Persistenz

Wenn eine neue Fähigkeit eingeführt wird, ist zu begründen, ob sie dem BIOS, Stammhirn oder neuronalen Zustand zugeordnet wird.

Ein dauerhaft relevanter neuronaler Zustand darf nicht nur als unsichtbare Variable im Forschungsrunner existieren.

Sämtliche neuen Zustände müssen:

- Vollständig persistierbar sein.
- Deterministisch wiederhergestellt werden.
- In diagnostischen Kopien erhalten bleiben.
- Bei deaktiviertem Mechanismus bestehende Regressionen unverändert lassen.

---

## 9. Phase F – Experimentelle Kontrollen

Mindestens folgende Gruppen sind vorzusehen:

| Gruppe | Beschreibung |
|---|---|
| K0 | Korrigierte B.4.8-Referenz |
| K1 | No-Learning |
| K2 | Feste inhibitorische Verbindungen |
| K3 | Lernbare inhibitorische Verbindungen |
| K4 | Rückmeldungsunabhängige Zusatzexploration |
| K5 | B.4.8-adaptive Exploration |
| K6 | Gleiche zusätzliche Ereigniszahl, andere zeitliche Verteilung |
| K7 | Bedingte lokale Fehlerkorrektur, sofern begründet |
| K8 | Identischer Aufbau mit deaktivierter neuer Funktion |

Die Versuchsgruppen müssen hinsichtlich Netzstruktur, Initialisierung, Lernzeit und Zufallsquellen vergleichbar sein.

Falls K7 nicht gerechtfertigt werden kann, wird diese Gruppe nicht implementiert.

Die Forschungsstufe kann ohne neuen Mechanismus abgeschlossen werden.

---

## 10. Phase G – Lernaufgaben

Alle zentralen Forschungsvarianten durchlaufen:

1. Einzelreiz X→A.
2. Einzelreiz Y→B.
3. X→A, danach Y→B.
4. Y→B, danach X→A.
5. Streng alternierende Erfahrungen.
6. Zufällig gemischte Erfahrungen.
7. Inverse Zielzuordnung X→B und Y→A.
8. Umkehr der Umweltbewertung nach erfolgreichem Lernen.
9. Retention ohne weitere Plastizität.

### Zusätzliche Fehlerkorrekturaufgaben

**G1 – Wiederholter Fehlreiz**

Eine zunächst falsch beantwortete Situation wird erneut präsentiert.

Untersucht wird, ob negative Erfahrung eine Änderung der autonomen Antwort ermöglicht.

**G2 – Zufällig erfolgreiche Alternative**

Eine andere Handlung wird durch allgemeine Exploration ausgelöst und positiv bewertet.

Untersucht wird, ob EVE anschließend eigenständig dieselbe erfolgreiche Antwort hervorbringen kann.

**G3 – Störung einer bereits erlernten Beziehung**

Die Umweltbewertung verändert sich, ohne dass das neuronale Netzwerk zurückgesetzt wird.

Untersucht wird die Entstehung neuer autonomer Antworten.

**G4 – Interferenzkontrolle**

Das Lernen einer Beziehung darf die andere nicht systematisch zerstören.

Die Versuche verwenden weiterhin keine neuronale Information über die extern richtige Handlung.

---

## 11. Hauptziel: Autonomes Mehrfachlernen

Die primäre Erfolgsmetrik bleibt E1.

Eine Entscheidung gilt als autonom erfolgreich, wenn allein die sensorische Reizpräsentation zu genau der richtigen motorischen Ausgabe führt.

Doppelte und fehlende Ausgaben zählen als Misserfolg.

### Erfolgskriterien

Erforderlich sind:

- Mindestens 75 % E1-Erfolg für X.
- Mindestens 75 % E1-Erfolg für Y.
- Mindestens 48 von 64 Seeds erreichen beide Schwellen gleichzeitig.
- Mindestens 15 Prozentpunkte gepaarter Vorteil gegenüber No-Learning.
- Vollständig positives 95-%-Konfidenzintervall.
- Reproduktion mit 64 unabhängigen Bestätigungs-Seeds.
- Erfolg bei normaler und inverser Zielzuordnung.
- Reproduktion über mindestens drei unabhängige Zufallsstromfamilien.
- Erhalt der Fähigkeit ohne weitere Plastizität.

E2 bleibt ausdrücklich sekundär.

### Zusätzlicher Nachweis für neue Mechanismen

Eine neue lokale Funktion gilt nur als lernförderlich, wenn sie gegenüber der unveränderten B.4.8-Referenz einen reproduzierbaren autonomen Vorteil zeigt.

Außerdem muss geklärt werden, ob die Verbesserung lediglich auf zusätzlicher Exploration beruht.

Eine Verbesserung bei gleicher Explorationsmenge und vergleichbarer zeitlicher Verteilung wäre besonders aussagekräftig.

---

## 12. Kontrollierte Prüfung des Umlernens

Neben dem ursprünglichen Lernen wird die Anpassung an geänderte Umweltbedingungen untersucht.

Dafür ist eine Beziehung zunächst zu erlernen.

Anschließend wird ausschließlich die Umweltbewertung verändert.

Der neuronale Zustand bleibt vollständig erhalten.

Erfasse:

- Autonomen Erfolg vor der Umkehr.
- Erste negative Rückmeldungen nach der Umkehr.
- Anzahl wiederholter alter Handlungen.
- Auftreten neuer Alternativhandlungen.
- Zeitpunkt der ersten positiv bewerteten Alternative.
- Veränderung synaptischer Gewichte.
- Entstehung neuer autonomer Antworten.
- Erhalt der zweiten Reiz-Handlungs-Beziehung.

Ein erfolgreicher Wechsel muss durch neuronales Lernen entstehen.

Es darf kein technischer Zustandsreset stattfinden.

---

## 13. Kausale Prüfung neuer Funktionen

Falls eine neue Forschungsvariante E1 verbessert, müssen gezielte Zustandskopie-Ablationen folgen.

Prüfe:

1. Verhalten mit neuer lokaler Funktion.
2. Verhalten bei deaktivierter Funktion.
3. Verhalten mit identischer zusätzlicher Ereigniszahl.
4. Verhalten nach Einfrieren der erworbenen Gewichte.
5. Verhalten nach Rücknahme einzelner relevanter Gewichtsänderungen.
6. Verhalten nach Deaktivierung inhibitorischer Wirkung.
7. Verhalten ohne weitere Plastizität.
8. Verhalten mit inversen Zielbeziehungen.

Dabei ist zwischen drei möglichen Ursachen zu unterscheiden:

- Zusätzliche neuronale Aktivität.
- Andere zeitliche Anordnung neuronaler Aktivität.
- Tatsächlich verbesserte erfahrungsabhängige Bindung.

Eine statistische Verbesserung allein ist kein ausreichender Nachweis des behaupteten Mechanismus.

---

## 14. Statistische Anforderungen

Die zentralen Experimente verwenden:

- Mindestens 64 gepaarte Entdeckungs-Seeds pro Stromfamilie und Zielzuordnung.
- Mindestens 64 zusätzliche Bestätigungs-Seeds.
- Mindestens drei vorab festgelegte unabhängige Stromfamilien.
- Kontrollierte Reiz-, Explorations- und Runtime-Zufallsquellen.
- Identische äußere Aufgaben bei gepaarten Vergleichen.
- Getrennte E1- und E2-Zustandskopien.

Erforderliche Auswertungen:

- Autonome Erfolgsquoten nach Reiz.
- Gleichzeitig erfolgreiche Seeds.
- Gepaarte Erfolgsdifferenzen.
- Bootstrap-Konfidenzintervalle.
- Verwechslungsmatrizen.
- Wiederholungsraten falscher Entscheidungen.
- Alternative Aktivierungen nach negativer Rückmeldung.
- Zeitpunkt der ersten erfolgreichen alternativen Erfahrung.
- Gewichtsentwicklung erregender und hemmender Synapsen.
- Vergleich der Ereignisverläufe vor und nach Fehlerkorrektur.
- Ressourcenverbrauch und Zustandsgröße.

Die primären Vergleiche werden vor den unabhängigen Bestätigungsläufen festgelegt.

Bei mehreren explorativen Kandidaten sind Mehrfachvergleiche entsprechend zu berücksichtigen.

---

## 15. Architekturgrenzen

Die bisherigen Grundsätze gelten unverändert:

- C++17.
- Vollständig ereignisgesteuerte Verarbeitung.
- Keine globalen neuronalen Aktualisierungstakte.
- Binäre Feuerereignisse.
- Gewichtete erregende und hemmende Synapsen.
- Lokale G3- und Eligibility-Verarbeitung.
- Drei-Faktor-Plastizität.
- Geschützte angeborene Exploration.
- Keine neuronalen Reizkennungen mit äußerer Semantik.
- Keine globale Handlungswahl.
- Keine technisch vorgegebene richtige Handlung.
- Keine versteckte motorische Alternativauswahl.
- Keine dauerhaften Zeitstempel pro Synapse.
- Keine automatische Löschung vorhandener Erfahrungen.
- Keine globale neuronale Zustandsabfrage zur Lernsteuerung.
- Vollständige Persistenz.
- Reproduzierbare Verarbeitung.

Synapsenwachstum bleibt in den zentralen Versuchen deaktiviert, um die Wirkung der Fehlerkorrektur isoliert untersuchen zu können.

---

## 16. Regression und Stabilität

Alle bisherigen 459 Kerntests aus B.4.8 müssen weiterhin bestehen.

Zusätzliche Tests sind erforderlich für:

- Vollständige Ereignisabfolge einer autonomen Fehlentscheidung.
- Lokale negative Modulation.
- Veränderung beteiligter Synapsengewichte.
- Fortbestehen und Erreichbarkeit alternativer Wege.
- Inhibitorische Konkurrenz nach negativer Erfahrung.
- Deterministische identische Wiederholung.
- Unabhängige E1-/E2-Evaluation.
- Inverse Zielzuordnung.
- Alternative Trainingsfortsetzungen.
- Zustandskopie-Ablationen.
- Gegebenenfalls neue lokale Fehlerkorrektur.
- Persistenz sämtlicher neuer neuronaler Zustände.

Unzulässig sind unkontrollierte Ereignisschleifen, unbeschränkt wachsende Zustände oder unbemerkte Veränderungen bisheriger Lernmechanismen.

---

## 17. Lieferumfang

Nova-EVE erstellt:

1. Technische Analyse der Verarbeitung einer negativen autonomen Entscheidung.
2. Reproduktion der B.4.8-Referenzen.
3. Korrekt getrennte E1-only- und E2-Evaluationen.
4. Vollständige Ereignisdiagnose ausgewählter Fehlentscheidungen.
5. Analyse erregender und inhibitorischer Konkurrenz.
6. Kausale Untersuchungen alternativer neuronaler Aktivierung.
7. Experimente zur Bindung positiv bewerteter Alternativen.
8. Gegebenenfalls eine begründete minimale lokale Fehlerkorrekturvariante.
9. Vollständige Kontrollgruppen.
10. Normale, inverse, gemischte und Umkehr-Lernexperimente.
11. Statistische Auswertungen mit unabhängigen Seeds und Stromfamilien.
12. Zustandskopie-Ablationen.
13. Persistenz- und Regressionstests.
14. Forschungsbericht B.4.9.
15. Entscheidungsvorlage für B.5.0.

Bisherige Forschungsergebnisse dürfen nicht überschrieben werden.

---

## 18. Abschlussentscheidung

Am Ende sind folgende Fragen zu beantworten:

**A.** Warum führt negative Rückmeldung nach einer autonomen Fehlentscheidung nicht zuverlässig zu einer anderen neuronalen Entscheidung?

**B.** Welche Rolle spielen dabei erregende Dominanz und inhibitorische Konkurrenz?

**C.** Ist die fehlende Alternativaktivierung oder die unzureichende Speicherung einer erfolgreich erprobten Alternative der größere Engpass?

**D.** Können vorhandene neuronale Fähigkeiten den Engpass bereits beheben?

**E.** Ist eine zusätzliche lokale Fähigkeit tatsächlich notwendig?

**F.** Verbessert eine gegebenenfalls eingeführte Funktion die autonome Lernleistung über unabhängige Erfahrungsströme hinweg?

**G.** Bleiben bereits erworbene Fähigkeiten erhalten?

**H.** Kann EVE eine erlernte Handlung nach veränderter Umweltbewertung eigenständig korrigieren?

Eine allgemeine Freigabe erfolgt nur bei Erfüllung der vorab definierten Kriterien.

Auch ein negativer Forschungsbefund ist zulässig.

---

## 19. Forschungsleitlinie

**Eine negative Erfahrung ist noch keine Korrektur.**

Sie kann neuronale Gewichte verändern, ohne dass daraus eine neue erfolgreiche Handlung entsteht.

Ebenso ist eine alternative Handlung noch kein Nachweis für Lernen, solange sie ausschließlich durch zufällige Exploration zustande kommt.

EVE soll aus negativen Erfahrungen ihre neuronalen Aktivitätsbedingungen verändern können, alternative Erfahrungen ermöglichen und daraus selbstständig abrufbare Verbindungen entwickeln.

Dabei darf das System nicht technisch wissen, welche Handlung die richtige ist.

**Das eigentliche Forschungsziel ist die Entstehung einer neuronalen Fähigkeit, eigenes Verhalten aufgrund neuer Erfahrungen zu korrigieren – nicht die Programmierung einer anderen Handlung nach einem Fehler.**
