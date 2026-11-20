# Technische Kausalanalyse – lokale Entscheidungsspielräume B.5.9

## Gegenstand

B.5.9 verbindet eine konkrete Gewichtsanwendung mit der postsynaptischen
Integration, der Feuer- oder Nichtfeuerentscheidung, der erzeugten Eligibility
und der späteren Gewichtsänderung. Die Diagnose verändert weder die
Produktionsregel noch den persistenten neuronalen Zustand.

## Ereignisnahe Messung

`ImpulseResult`, `ImpulseBatchResult` und die ausschließlich beobachtende
`ImpulseObservation` erfassen nun:

- Membran vor und am Entscheidungszeitpunkt,
- Feuergrenze und vorzeichenbehaftete Entscheidungsmarge,
- getrennte erregende, hemmende und Netto-Beiträge,
- Refraktärzustand vor und nach der Verarbeitung,
- Feuerung, Unterdrückung, Impulsbündel und Reihenfolge,
- beteiligte Synapsen und tatsächlich angewandte Gewichte.

Die Entscheidungsmarge ist `Membran am Entscheid - Feuergrenze`. Sie wird nach
der begrenzten Integration und vor einem möglichen Membranreset gebildet. Bei
27.913 beobachteten Integrationsereignissen feuerten alle 5.148 positiven und
alle 7.140 Nullmargen. 14.640 negative Margen feuerten nicht; weitere 985
Entscheidungen wurden refraktär unterdrückt. Eligibility entstand genau bei
den 12.288 Feuerentscheidungen, nicht bei Nichtfeuerentscheidungen.

## Nichtfeuerentscheidungen

Die 15.625 Nichtfeuerereignisse teilen sich wie folgt auf:

| Grund | Ereignisse |
|---|---:|
| hemmungsdominant | 9.087 |
| unterschwellige Integration | 3.373 |
| keine wirksame Erregung | 2.168 |
| refraktär unterdrückt | 985 |
| unzureichende Bündelerregung | 12 |

Die Klassifikation erklärt die lokale Integrationslage, aber nicht die global
richtige Handlung. Insbesondere bedeutet ein hemmungsdominanter Impuls nicht,
dass die Hemmung für die fehlende Antwort kausal notwendig war.

## Eingangablationen

Für 96 präregistrierte Zustände wurden identische Kopien verwendet:

| Intervention | Fälle | Feuerung geändert | Marge geändert | Handlungskorrektheit geändert |
|---|---:|---:|---:|---:|
| K6 einzelner erregender Beitrag | 96 | 15 | 30 | 15 |
| K7 einzelner hemmender Beitrag | 96 | 0 | 0 | 0 |
| K8 hemmend plus gepaarter erregender Ausgang | 96 | 0 | 0 | 0 |

K7 und K8 sind ausgeführte Nullbefunde. Die beobachteten inhibitorischen
Ausgangsimpulse trafen jeweils eine Motor-Membran von 0; die nichtnegative
Membranbegrenzung machte ihre Entfernung wirkungslos. Ein vorhandener und
gewichteter Eingang ist somit nicht automatisch ein kausal wirksamer Beitrag.

## Verbindung zur Plastizität

17.585 Kandidatenanwendungen ließen sich auf ihre Feuerentscheidung
zurückführen. Für negative Modulation wurden 3.488 Beobachtungen erfasst,
davon 2.484 mit tatsächlicher Gewichtsänderung. Für positive Modulation waren
es 14.097 Beobachtungen und 2.467 Änderungen. Positive Anwendungen waren
1.362-mal unmittelbar nützlich, 3.160-mal unmittelbar schädlich, 5.170-mal
unmittelbar neutral und 4.405-mal erst in der festgelegten Folgeprüfung
verändert. Entscheidungsmarge und Beitragsstärke sind daher diagnostisch
nützlich, aber keine hinreichende Wertinformation.

## Informationsstufen

- L0: B.5.8-Signatur am Modulationszeitpunkt,
- L1: zusätzlich Erregung, Hemmung und Netto-Eingang,
- L2: zusätzlich Entscheidungsmarge, Feuerung und Refraktärzustand,
- L3: zusätzlich Ereignisabstand, Gleichzeitigkeit und Bündelposition,
- L4: zusätzlich Kandidatenbeitrag, relativer Anteil und lokale Konkurrenz.

Auf der unabhängigen Bestätigung steigt die Vorhersagegenauigkeit von 49,49 %
bei L0 auf 53,51 % bei L3/L4. Der gepaarte L4-L0-Unterschied beträgt 4,23
Prozentpunkte (seed-geclustertes 95-%-Bootstrapintervall 2,78–5,91). Trotzdem
liegen bei L4 weiterhin 90,50 % der Beobachtungen in Signaturen mit gemischten
Folgen. L4 trennt keine zusätzliche Signatur gegenüber L3. Die zusätzliche
Information verbessert also eine statistische Prognose, liefert aber keinen
sicheren lokalen Selektor.

## Historische Fälle

Die zwölf B.5.6-Fälle wurden diesmal ab dem ersten gemeinsamen Erwerb verfolgt.
Jeder erste Verlust trat nach positiven und ohne negative Änderungen auf. Am
Verlustpunkt waren insgesamt 52 Gewichte gegenüber dem ersten gemeinsamen
Zustand erhöht. Eine isolierte Rücknahme stellte in 10/52 Fällen beide
Beziehungen wieder her; darunter waren acht erregende und zwei inhibitorische
Synapsen. Die kombinierte Rücknahme stellte 12/12 Fälle wieder her.

Diese Rollbacks sind reine Kausaldiagnostik. Sie belegen konkrete schädliche
positive Konkurrenz, rechtfertigen aber weder automatische Sicherung noch
Rücknahme erlernter Gewichte.

## Stabilität, Umkehr und Zeit

Die B.5.8-Kontrollen wurden erhalten. Nach 512 Erfahrungen waren 62/96 aktive
und 90/96 schreibgesperrte Kopien robust. Nach Umkehr waren 60/96 aktive,
0/96 weiter gesperrte und 61/96 reaktivierte Kopien robust. Fest-1 und Fest-12
erreichten 0/384, Fest-16 240/384 beziehungsweise 244/384, Fest-32 259/384 und
unregelmäßige Abstände 156/384.

## Persistenz und Ressourcen

Der Beobachter fügt keinen persistenten Zustand hinzu; Persistenzversion 15
bleibt unverändert. Laden und Fortsetzung waren fingerprint-identisch. Der
Lauf verarbeitete 4.224 zentrale Netze, 96 lokale Quellen, 4.522 spätere
Diagnosekopien und 288 Eingangablationen. Die maximale Warteschlange betrug
27 Ereignisse, der größte geschätzte Runtime-Speicher 25.120 Byte. Alle
Zustände waren gültig.

## Technische Entscheidung

Die ereignisnahe Diagnose wird integriert. K21/K22, automatische
Stabilitäts-Gates und eine neue lokale Lernregel werden nicht implementiert.
S4 und die vorhandene Schreibsperre bleiben standardmäßig deaktivierte
Forschungsmechanismen.
