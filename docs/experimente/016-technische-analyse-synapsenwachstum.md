# Technische Analyse – Synapsenwachstum in B.4.4

## Auslösung und Auswahl

`Runtime::process_fire` versucht nach dem Versand aller bereits vorhandenen
Ausgänge genau einmal Wachstum, sofern `growth_enabled` aktiv ist. Bei `k`
Ausgängen beträgt die Versuchswahrscheinlichkeit `1/(1+k)`; bei `k=0` ist der
Versuch sicher. Der aktuelle Feuerimpuls läuft damit noch nicht über die neu
erzeugte Verbindung.

B.4.4 trennt dafür erstmals den persistierten Wachstums-Zufallsstrom vom
allgemeinen Laufzeit-/Plastizitätsstrom. Wachstum kann somit weder Aufgaben-,
Explorations- und Feedbackpläne noch zufällige Plastizitätsauswahl verschieben.

`Brainstem::attempt_growth` durchsucht vom Sender aus Chebyshev-Schalen mit
Radius 1 bis zur maximalen Netzausdehnung. Es wählt aus der ersten Schale mit
freien Zielen reproduzierbar zufällig. Selbstverbindungen und vorhandene
Sender-Empfänger-Paare sind ausgeschlossen. Sind alle Ziele belegt, endet der
Versuch ohne Verbindung. Zielhandlung, Belohnung, Reizbedeutung und
Handlungskennung gehen nicht in die Auswahl ein.

## Anfangszustand und Lernfähigkeit

Eine neue Synapse entsteht mit Gewicht 0. Sie überträgt bei späterem Feuer des
Senders einen Impuls mit Wert 0. Dieser Impuls kann dennoch lokale Aktivität
und G3-Ereignisse erzeugen und – abhängig von der gewählten lokalen
Eligibility-Selektion – Kandidat einer Drei-Faktor-Änderung werden. Erst eine
positive Modulation kann daraus Gewicht 1 und damit einen erregenden Lernweg
machen. Negative Modulation kann ein Nullgewicht nicht weiter absenken.

Wachstum schafft daher zunächst Struktur, nicht Funktion. Zugleich kann jede
Nullgewicht-Verbindung die Zahl lokaler Kandidaten erhöhen und damit bei
begrenzter Kandidatenkapazität mit vorhandenen Wegen konkurrieren.

## Grenzen und Zustand

Das bestehende Verfahren hat kein separates globales Synapsenbudget und keine
Entfernung. Natürliche Grenzen sind höchstens ein gerichtetes Paar pro zwei
verschiedenen Neuronen, die sinkende Wahrscheinlichkeit bei wachsendem `k`,
die endliche Zielmenge und die bestehenden Speicher-/ID-Grenzen. Gewichte
bleiben an die konfigurierte Obergrenze gebunden. Topologie, Zufallszustand,
Wachstumsschalter und Metriken werden persistiert.

Für B.4.4 kann Wachstum vor der plastizitätsfreien Auswertung über
`set_structural_growth_enabled(false)` gestoppt werden. Die Methode verändert
keine Synapse und ist nur im inaktiven motorischen Zustand zulässig.

## Beobachteter Mechanismus im Versuch

S1 erzeugte in den Sequenzaufgaben im Mittel etwa 104 und im gemischten Lauf
etwa 112 neue Verbindungen. Viele wurden später mit Nullimpulsen aktiviert,
aber keine einzige dynamisch gewachsene Synapse erreichte ein Gewicht über 0.
S1 verhielt sich deshalb Seed für Seed exakt wie S0. S4 verhielt sich trotz
derselben Größenordnung von Wachstum exakt wie S3.

S2 trennte reine Zusatzkapazität von zeitlicher Entstehung. Die vorab
angelegten Mengen wurden ausschließlich an die mittlere S1-Strukturgröße je
Aufgabe angepasst, nie an Verhaltensergebnisse. Nur sehr wenige statische
Verbindungen wurden lernwirksam; es entstand kein belastbarer Vorteil.

Damit erzeugt der vorhandene Mechanismus in dieser Aufgabe zusätzliche
Kandidaten und Ressourcenverbrauch, aber keine nachgewiesenen unabhängigen
Lernwege.
