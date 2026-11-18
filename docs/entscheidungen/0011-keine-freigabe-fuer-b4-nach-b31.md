# 0011 – Keine Freigabe für B.4 nach B.3.1

**Status:** Angenommen

**Datum:** 2026-11-18

## Kontext

B.3.1 implementiert neuronale Handlungsgrenzen, motorische Eigenaktivität, allgemeine Kontextkreise, begrenzte lokale Kontextspuren und zwei lokale Bindungsregeln für Eligibility. Die Mechanismen verwenden keine technische Handlungs-ID und verändern keine Gewichte.

Die 64-Seed-Einzelaktionsmatrix belegt, dass C3/C4 mit B1/B2 Kontext und Eligibility tatsächlich zusammenführen können. Die beste Variante C4/P2/B2 erreichte bei einer einzelnen konkurrenzlosen Episode 43 von 64 Zuordnungen.

Im entscheidenden Zwei-Handlungs-Versuch waren jedoch nur 96 von 2.688 Rückmeldungen eindeutig richtig. 18 waren eindeutig falsch, 1.447 mehrdeutig und 1.127 nicht zugeordnet. Bei vertauschter Rückmeldungsreihenfolge R3 traten alle 18 eindeutigen Fehlzuordnungen auf. Identische Folgehandlungen blieben überwiegend mehrdeutig.

## Entscheidung

B.4 mit rückmeldungsabhängiger synaptischer Gewichtsänderung wird nicht freigegeben.

Kontextspuren, Bindungsfelder, Herkunftstrennung, Persistenz und Diagnostik bleiben als Forschungsinfrastruktur erhalten. Sie dürfen bis zu einer neuen ausdrücklichen Entscheidung keine positive oder negative Gewichtsänderung auslösen.

## Begründung

Eine Lernregel benötigt nicht nur lokale Erreichbarkeit, sondern hinreichend zuverlässige Zuordnung. Die aktuelle Qualität erfüllt diese Voraussetzung nicht:

- 3,57 % eindeutige richtige Zuordnung im Zwei-Handlungs-Versuch,
- 53,83 % mehrdeutige Zuordnung,
- 41,93 % ohne Zuordnung,
- 0,67 % eindeutige Fehlzuordnung,
- fehlende Unterscheidbarkeit neuronal identischer Folgehandlungen,
- 18 Fehlzuordnungen bei vertauschter Rückmeldung.

Eine Gewichtsänderung auf dieser Grundlage würde die noch offene zeitliche Zuordnung in dauerhafte Netzstruktur übersetzen. Herkunftstrennung allein verhindert dies nicht.

## Folgerung für eine mögliche Folgestufe

Vor B.4 ist eine weitere reine Zuordnungsforschung erforderlich. Sie darf keine technische Episodennummer einführen. Zu prüfen wären insbesondere neuronal erzeugte Reihenfolgeinformation, konkurrierende lokale Kontexte und ein explizites neuronales Abbruchsignal für mehrdeutige Rückmeldung.

Vor Beginn sind quantitative Freigabekriterien festzulegen, darunter:

- eindeutige Trefferquote getrennt für unterschiedliche und identische Handlungen,
- maximal zulässige Fehlzuordnung bei R2 und R3,
- Verhalten nach Ablauf und bei fehlender Rückmeldung,
- Ausschluss rückmeldungsinduzierter Eligibility und Kontexte,
- Stabilität über mindestens 128 Episoden,
- unveränderte Gewichte bis zur gesonderten B.4-Entscheidung.

## Nachweise

- [Forschungsbericht B.3.1](../experimente/011-forschungsbericht-forschungsstufe-b31.md)
- [Technische Spezifikation](../konzept/EVE-Neuronaler-Handlungskontext.md)
- `research/results/research-stage-b31-contexts.csv`
- `research/results/research-stage-b31-sequences.csv`
- `research/results/research-stage-b31-controls.csv`
- `research/results/research-stage-b31-continuous.csv`
- `research/results/research-stage-b31-persistence.csv`
- `research/results/research-stage-b31-performance.csv`
