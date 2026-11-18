# Entscheidung 0015: Keine Freigabe für Mehrfachlernen nach B.4.3

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidungen 0012–0014

## Kontext

B.4.3 reproduziert das Einzelreiz- und Umkehrlernen, weist aber starke lokale
Interferenz bei zwei Reizen nach. Im G3-Referenzfall stammten 30,15 % der
sensorisch klassifizierbaren Eligibility-Kandidaten aus der jeweils anderen
Erfahrung. Sequenzielles Lernen löschte die zuerst erworbene Beziehung um mehr
als 80 Prozentpunkte.

Die diagnostische Rücknahme der im zweiten Block entstandenen
Gewichtsänderungen stellte die erste Fähigkeit in allen 64 Zustandskopien
wieder her. Eine lokale Feuerfenstergrenze reduzierte vermischte Kandidaten,
verbesserte das Verhalten aber nicht. Keine Variante erreichte gleichzeitig
75 % für beide Beziehungen.

## Entscheidung

Die vorhandene Drei-Faktor-Plastizität wird nicht für allgemeines
Mehrfachlernen freigegeben. Geschützte Exploration und das nachgewiesene
Einzelreiz-Lernen bleiben gültig; die neue `active_phase`-Auswahl bleibt als
isolierte Forschungsvariante verfügbar, nicht als Standard.

Es wird keine globale Reizkennung, Handlungs-ID, Episodennummer oder
technische Zielauswahl in den neuronalen Zustand eingeführt. Ebenso wird noch
kein synaptisches Budget-, Lösch-, Hemmungs- oder allgemeines STDP-Verfahren
freigegeben.

## Vorlage für die nächste Forschungsstufe

Die nächste Stufe soll eine lokale Koinzidenzbindung untersuchen:

1. mehrere Eingänge, die gemeinsam zur postsynaptischen Entscheidung
   beitrugen, als eine lokale Entscheidungseinheit abgrenzen;
2. Nullgewicht-Synapsen weiterhin lernfähig halten;
3. Modulation ausschließlich an die noch aktuelle Entscheidungseinheit
   binden, ohne technische Reizbedeutung;
4. dieselben B.4.2-Einzelreizregressionen, B.4.3-Verwechslungsmatrizen,
   Sequenz-/Vergessensmaße und Kausalablationen wiederholen;
5. eine Übernahme nur bei gleichzeitig mindestens 75 % für X und Y,
   mindestens 15 Prozentpunkten gepaartem Vorteil und positivem
   95-%-Intervall erwägen.

## Folgen

- B.4.3 ist eine abgeschlossene negative/diagnostische Forschungsstufe.
- Mehrfachlernen und Kapazität über zwei Reize bleiben offen.
- Längere Eligibility-Dauern gelten bei Episodenabstand 16 als nachweislich
  interferenzverstärkend.
- Zusätzliche Netzgröße allein ist keine zulässige Ersatzlösung.
- Die Produktionssperre der Entscheidungen 0012–0014 bleibt bestehen.

