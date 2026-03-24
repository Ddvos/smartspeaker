import type { Conversation } from '$lib/types';

const now = new Date();
const daysAgo = (d: number) => new Date(now.getTime() - d * 86400000);
const hoursAgo = (h: number) => new Date(now.getTime() - h * 3600000);
const minutesAgo = (m: number) => new Date(now.getTime() - m * 60000);

export const mockConversations: Conversation[] = [
	{
		id: 'conv-1',
		title: 'Slimme verlichting instellen',
		createdAt: hoursAgo(3),
		updatedAt: hoursAgo(2),
		mood: 'focused',
		duration: 540,
		tags: ['domotica', 'zigbee'],
		summary:
			'Gesprek over het instellen van Zigbee-lampen via Home Assistant. Besproken hoe je groepen maakt en automatiseringen instelt voor zonsondergang.',
		messages: [
			{
				id: 'msg-1-1',
				role: 'user',
				content: 'Ik wil mijn Zigbee-lampen koppelen aan Home Assistant. Hoe begin ik daarmee?',
				timestamp: hoursAgo(3)
			},
			{
				id: 'msg-1-2',
				role: 'assistant',
				content:
					'Leuk dat je daarmee aan de slag gaat! Eerst moet je zorgen dat je een Zigbee-coordinator hebt, zoals een Conbee II of Sonoff Zigbee dongle. Die sluit je aan op je Home Assistant server. Daarna installeer je de ZHA-integratie of Zigbee2MQTT via de instellingen.',
				timestamp: hoursAgo(3)
			},
			{
				id: 'msg-1-3',
				role: 'user',
				content: 'Ik heb een Sonoff dongle. Welke optie raad je aan, ZHA of Zigbee2MQTT?',
				timestamp: hoursAgo(2.8)
			},
			{
				id: 'msg-1-4',
				role: 'assistant',
				content:
					'Voor de meeste gebruikers raad ik Zigbee2MQTT aan. Het is iets flexibeler, heeft betere ondersteuning voor veel apparaten en je kunt het makkelijker debuggen. Je installeert het als add-on in Home Assistant en configureert je Sonoff dongle als coordinator.',
				timestamp: hoursAgo(2.7)
			},
			{
				id: 'msg-1-5',
				role: 'user',
				content: 'Kan ik dan ook groepen maken van lampen in dezelfde kamer?',
				timestamp: hoursAgo(2.5)
			},
			{
				id: 'msg-1-6',
				role: 'assistant',
				content:
					'Ja, absoluut! In Zigbee2MQTT kun je Zigbee-groepen aanmaken. Dat is efficienter dan Home Assistant groepen, want het commando gaat direct naar alle lampen tegelijk via het Zigbee-netwerk. Ga naar het Zigbee2MQTT dashboard, klik op "Groepen" en voeg je lampen toe.',
				timestamp: hoursAgo(2.3)
			},
			{
				id: 'msg-1-7',
				role: 'user',
				content:
					'Top, en hoe maak ik een automatisering dat de lampen aangaan bij zonsondergang?',
				timestamp: hoursAgo(2.1)
			},
			{
				id: 'msg-1-8',
				role: 'assistant',
				content:
					'Ga naar Instellingen > Automatiseringen in Home Assistant. Maak een nieuwe automatisering met als trigger "Zon: zonsondergang". Je kunt een offset instellen, bijvoorbeeld -30 minuten zodat de lampen al aan gaan als het begint te schemeren. Als actie kies je "Lamp inschakelen" en selecteer je je groep. Je kunt ook een overgang instellen van bijvoorbeeld 5 minuten zodat de lampen langzaam aan gaan.',
				timestamp: hoursAgo(2)
			}
		]
	},
	{
		id: 'conv-2',
		title: 'Pasta carbonara recept',
		createdAt: hoursAgo(8),
		updatedAt: hoursAgo(7),
		mood: 'happy',
		duration: 420,
		tags: ['koken', 'italiaans'],
		summary:
			'Een gedetailleerd recept voor authentieke pasta carbonara besproken. Tips gegeven over het gebruik van guanciale en pecorino, en het belang van de juiste temperatuur.',
		messages: [
			{
				id: 'msg-2-1',
				role: 'user',
				content:
					'Ik wil vanavond een echte Italiaanse pasta carbonara maken. Heb je een goed recept?',
				timestamp: hoursAgo(8)
			},
			{
				id: 'msg-2-2',
				role: 'assistant',
				content:
					'Wat een goed plan! Voor een authentieke carbonara heb je nodig: 400g spaghetti, 200g guanciale (of pancetta als je geen guanciale kunt vinden), 4 eidooiers, 100g Pecorino Romano, versgemalen zwarte peper en een snufje zout voor het pastawater. Let op: er gaat geen room in een echte carbonara!',
				timestamp: hoursAgo(7.9)
			},
			{
				id: 'msg-2-3',
				role: 'user',
				content: 'Geen room? Hoe wordt het dan zo romig?',
				timestamp: hoursAgo(7.7)
			},
			{
				id: 'msg-2-4',
				role: 'assistant',
				content:
					'Het geheim zit in de techniek! Je klopt de eidooiers met de geraspte pecorino tot een glad mengsel. De warmte van de net afgegoten pasta en het vet van de guanciale zorgen ervoor dat het ei zachtjes stolt tot een romige saus. Het belangrijkste is dat je de pan van het vuur haalt voordat je het eimengsel toevoegt, anders krijg je roerei!',
				timestamp: hoursAgo(7.5)
			},
			{
				id: 'msg-2-5',
				role: 'user',
				content: 'Goed om te weten! En hoe bak ik de guanciale het beste?',
				timestamp: hoursAgo(7.3)
			},
			{
				id: 'msg-2-6',
				role: 'assistant',
				content:
					'Snijd de guanciale in blokjes of reepjes en bak ze op middelhoog vuur in een droge pan, dus zonder olie. Het vet smelt er vanzelf uit. Bak het ongeveer 8-10 minuten tot het knapperig en goudbruin is. Bewaar het uitgebakken vet in de pan, want dat gebruik je voor de saus. Dat vet geeft de carbonara zijn kenmerkende smaak.',
				timestamp: hoursAgo(7)
			}
		]
	},
	{
		id: 'conv-3',
		title: 'Weer vandaag in Amsterdam',
		createdAt: daysAgo(1),
		updatedAt: daysAgo(1),
		mood: 'neutral',
		duration: 180,
		tags: ['weer', 'amsterdam'],
		summary:
			'Korte vraag over het weer in Amsterdam. Het was bewolkt met kans op regen in de middag, temperatuur rond 14 graden.',
		messages: [
			{
				id: 'msg-3-1',
				role: 'user',
				content: 'Wat voor weer wordt het vandaag in Amsterdam?',
				timestamp: daysAgo(1)
			},
			{
				id: 'msg-3-2',
				role: 'assistant',
				content:
					'Vandaag in Amsterdam wordt het overwegend bewolkt met in de middag kans op een bui. De temperatuur komt uit op ongeveer 14 graden. Er staat een matige westenwind. Neem voor de zekerheid een paraplu mee als je de deur uit gaat!',
				timestamp: daysAgo(1)
			},
			{
				id: 'msg-3-3',
				role: 'user',
				content: 'En morgen? Ik wil misschien gaan fietsen.',
				timestamp: daysAgo(1)
			},
			{
				id: 'msg-3-4',
				role: 'assistant',
				content:
					'Morgen ziet er beter uit! De zon komt vaker tevoorschijn en het wordt rond de 16 graden. De wind neemt af naar zwak. Prima fietsweer dus! De ochtend start wel fris, dus een extra laagje kan geen kwaad als je vroeg vertrekt.',
				timestamp: daysAgo(1)
			}
		]
	},
	{
		id: 'conv-4',
		title: 'Machine learning uitleg',
		createdAt: daysAgo(2),
		updatedAt: daysAgo(2),
		mood: 'curious',
		duration: 720,
		tags: ['ai', 'machine-learning'],
		summary:
			'Uitleg over de basisprincipes van machine learning. Van supervised learning tot neural networks, met praktische voorbeelden uitgelegd.',
		messages: [
			{
				id: 'msg-4-1',
				role: 'user',
				content:
					'Kun je me uitleggen wat machine learning precies is? Ik hoor het overal maar snap het niet helemaal.',
				timestamp: daysAgo(2)
			},
			{
				id: 'msg-4-2',
				role: 'assistant',
				content:
					'Machine learning is een onderdeel van kunstmatige intelligentie waarbij computers leren van data, zonder dat ze expliciet geprogrammeerd worden voor elke taak. Stel je voor: in plaats van een computer regels te geven zoals "als de e-mail het woord gratis bevat, is het spam", geef je de computer duizenden voorbeelden van spam en niet-spam. De computer ontdekt zelf de patronen.',
				timestamp: daysAgo(2)
			},
			{
				id: 'msg-4-3',
				role: 'user',
				content: 'Oké, en wat is dan het verschil tussen supervised en unsupervised learning?',
				timestamp: daysAgo(2)
			},
			{
				id: 'msg-4-4',
				role: 'assistant',
				content:
					'Bij supervised learning geef je het model gelabelde data. Bijvoorbeeld: foto\'s van katten en honden, waarbij je bij elke foto vertelt wat het is. Het model leert het verschil. Bij unsupervised learning heeft de data geen labels. Het model zoekt zelf structuur en patronen. Denk aan het groeperen van klanten op basis van koopgedrag, zonder dat je van tevoren zegt welke groepen er zijn.',
				timestamp: daysAgo(2)
			},
			{
				id: 'msg-4-5',
				role: 'user',
				content: 'En waar passen neural networks hierin?',
				timestamp: daysAgo(2)
			},
			{
				id: 'msg-4-6',
				role: 'assistant',
				content:
					'Neural networks zijn een specifiek type model dat geïnspireerd is op de werking van het menselijk brein. Ze bestaan uit lagen van "neuronen" die verbonden zijn met gewichten. Elke laag leert een ander abstractieniveau. Bij beeldherkenning leert de eerste laag randen, de tweede vormen, en diepere lagen herkennen complexe objecten. Als je heel veel lagen hebt, noemen we het deep learning.',
				timestamp: daysAgo(2)
			},
			{
				id: 'msg-4-7',
				role: 'user',
				content: 'Wauw, en is ChatGPT dan ook een neural network?',
				timestamp: daysAgo(2)
			},
			{
				id: 'msg-4-8',
				role: 'assistant',
				content:
					'Ja, precies! ChatGPT is gebaseerd op een transformer-architectuur, wat een speciaal type neural network is. Het is getraind op enorme hoeveelheden tekst van het internet. Het bijzondere aan transformers is het "attention-mechanisme": het model kan begrijpen welke woorden in een zin belangrijk zijn voor elk ander woord, waardoor het context goed begrijpt. Daarom kan het zulke natuurlijke teksten genereren.',
				timestamp: daysAgo(2)
			}
		]
	},
	{
		id: 'conv-5',
		title: 'Spotify afspeellijst maken',
		createdAt: daysAgo(3),
		updatedAt: daysAgo(3),
		mood: 'playful',
		duration: 360,
		tags: ['muziek', 'spotify'],
		summary:
			'Hulp bij het samenstellen van een chill afspeellijst op Spotify. Suggesties gegeven voor lo-fi, jazz en ambient artiesten.',
		messages: [
			{
				id: 'msg-5-1',
				role: 'user',
				content:
					'Ik wil een chille afspeellijst maken op Spotify voor als ik werk. Heb je suggesties?',
				timestamp: daysAgo(3)
			},
			{
				id: 'msg-5-2',
				role: 'assistant',
				content:
					'Leuk! Wat voor sfeer zoek je? Meer lo-fi hiphop beats, rustige jazz, ambient elektronica, of een mix van alles? En hoe lang wil je de lijst hebben, een paar uur of een hele werkdag?',
				timestamp: daysAgo(3)
			},
			{
				id: 'msg-5-3',
				role: 'user',
				content: 'Een mix van lo-fi en jazz zou ideaal zijn. Zo\'n 3-4 uur.',
				timestamp: daysAgo(3)
			},
			{
				id: 'msg-5-4',
				role: 'assistant',
				content:
					'Top keuze! Hier zijn mijn suggesties. Voor lo-fi: Nujabes, J Dilla, Tomppabeats, en de Lofi Girl kanalen. Voor jazz: Chet Baker, Bill Evans, en als je iets moderners wilt: GoGo Penguin en Portico Quartet. Je kunt ook artiesten als Khruangbin en Tom Misch toevoegen, die zitten precies op het snijvlak van jazz en lo-fi.',
				timestamp: daysAgo(3)
			},
			{
				id: 'msg-5-5',
				role: 'user',
				content: 'Geweldig, Nujabes ken ik al. De rest ga ik opzoeken!',
				timestamp: daysAgo(3)
			},
			{
				id: 'msg-5-6',
				role: 'assistant',
				content:
					'Als je van Nujabes houdt, ga je GoGo Penguin en Khruangbin zeker waarderen! Pro-tip: gebruik de Spotify functie "Vergelijkbare nummers toevoegen" als je een paar nummers hebt die je bevallen. Dan vult Spotify de lijst aan met vergelijkbare tracks. Zo ontdek je ook weer nieuwe artiesten. Veel luisterplezier!',
				timestamp: daysAgo(3)
			}
		]
	},
	{
		id: 'conv-6',
		title: 'Reisplanning Japan',
		createdAt: daysAgo(5),
		updatedAt: daysAgo(4),
		mood: 'curious',
		duration: 900,
		tags: ['reizen', 'japan'],
		summary:
			'Uitgebreide planning voor een reis naar Japan. Route via Tokyo, Kyoto en Osaka besproken met tips over Japan Rail Pass, eten en bezienswaardigheden.',
		messages: [
			{
				id: 'msg-6-1',
				role: 'user',
				content:
					'Ik wil in het najaar twee weken naar Japan. Waar moet ik beginnen met plannen?',
				timestamp: daysAgo(5)
			},
			{
				id: 'msg-6-2',
				role: 'assistant',
				content:
					'Wat een fantastische bestemming! Het najaar is trouwens perfect door de prachtige herfstkleuren. Laten we beginnen met de basis. Voor twee weken raad ik een route aan langs drie hoofdsteden: Tokyo (5 dagen), Kyoto (4 dagen) en Osaka (3 dagen), met tussenstops in Hakone of Nara. Heb je al een budget in gedachten?',
				timestamp: daysAgo(5)
			},
			{
				id: 'msg-6-3',
				role: 'user',
				content: 'Rond de 3000 euro exclusief vliegticket. Is dat genoeg?',
				timestamp: daysAgo(5)
			},
			{
				id: 'msg-6-4',
				role: 'assistant',
				content:
					'Dat is prima te doen! Een paar tips om je budget goed te besteden: koop een Japan Rail Pass voor 14 dagen, dat is ongeveer 450 euro maar scheelt enorm op treinkosten, vooral de Shinkansen. Verblijf in een mix van hotels en traditionele ryokans. Eten kan heel betaalbaar in Japan: een kom ramen kost rond de 7 euro, en de conbini (gemakswinkels) hebben verrassend goede maaltijden.',
				timestamp: daysAgo(5)
			},
			{
				id: 'msg-6-5',
				role: 'user',
				content: 'Wat zijn de must-see plekken in Tokyo?',
				timestamp: daysAgo(4.5)
			},
			{
				id: 'msg-6-6',
				role: 'assistant',
				content:
					'In Tokyo moet je zeker naar: Shinjuku voor de skyline en het nachtleven, Shibuya voor de beroemde kruising en Harajuku voor de streetfashion. Bezoek de Senso-ji tempel in Asakusa, het TeamLab Borderless museum voor digitale kunst, en de Meiji-schrijn voor rust in het midden van de stad. Tsukiji Outer Market is perfect voor verse sushi als ontbijt!',
				timestamp: daysAgo(4.5)
			},
			{
				id: 'msg-6-7',
				role: 'user',
				content: 'En in Kyoto? Ik hou van tempels en natuur.',
				timestamp: daysAgo(4)
			},
			{
				id: 'msg-6-8',
				role: 'assistant',
				content:
					'Dan ga je Kyoto geweldig vinden! De Fushimi Inari-schrijn met duizenden rode torii-poorten is adembenemend, vooral vroeg in de ochtend als het rustig is. Het Arashiyama bamboebos is magisch, en de Kinkaku-ji (Gouden Paviljoen) is prachtig met herfstkleuren. Loop ook door de Gion-wijk voor de traditionele sfeer en misschien spot je zelfs een geiko. Het Philosopherspad is een heerlijke wandeling langs tempels en bomen.',
				timestamp: daysAgo(4)
			}
		]
	},
	{
		id: 'conv-7',
		title: 'Python debugging hulp',
		createdAt: daysAgo(6),
		updatedAt: daysAgo(6),
		mood: 'focused',
		duration: 480,
		tags: ['programmeren', 'python'],
		summary:
			'Hulp bij het debuggen van een Python-script dat een CSV-bestand verwerkt. Het probleem was een encoding-fout bij het lezen van speciale tekens.',
		messages: [
			{
				id: 'msg-7-1',
				role: 'user',
				content:
					'Mijn Python-script crasht als ik een CSV-bestand probeer te lezen. Ik krijg een UnicodeDecodeError.',
				timestamp: daysAgo(6)
			},
			{
				id: 'msg-7-2',
				role: 'assistant',
				content:
					'Dat is een veelvoorkomend probleem! De kans is groot dat het CSV-bestand niet in UTF-8 is opgeslagen. Hoe open je het bestand nu? Gebruik je de ingebouwde open() functie of pandas?',
				timestamp: daysAgo(6)
			},
			{
				id: 'msg-7-3',
				role: 'user',
				content:
					'Ik gebruik pandas met pd.read_csv("data.csv"). Het bestand komt van een Nederlandse overheidssite.',
				timestamp: daysAgo(6)
			},
			{
				id: 'msg-7-4',
				role: 'assistant',
				content:
					'Ah, Nederlandse overheidsdata is vaak opgeslagen in latin-1 of cp1252 encoding vanwege speciale tekens zoals e-trema en andere diakritische tekens. Probeer: pd.read_csv("data.csv", encoding="latin-1"). Als dat niet werkt, kun je ook encoding="cp1252" proberen. Een andere optie is om eerst de encoding te detecteren met het chardet pakket.',
				timestamp: daysAgo(6)
			},
			{
				id: 'msg-7-5',
				role: 'user',
				content: 'encoding="latin-1" werkt! Maar nu zijn sommige kolommen raar geformateerd.',
				timestamp: daysAgo(6)
			},
			{
				id: 'msg-7-6',
				role: 'assistant',
				content:
					'Dat komt waarschijnlijk door het scheidingsteken. Nederlandse CSV-bestanden gebruiken vaak een puntkomma in plaats van een komma als scheidingsteken, omdat wij de komma als decimaalteken gebruiken. Probeer: pd.read_csv("data.csv", encoding="latin-1", sep=";", decimal=","). Daarmee vertel je pandas dat het scheidingsteken een puntkomma is en dat komma\'s als decimaaltekens moeten worden gelezen.',
				timestamp: daysAgo(6)
			}
		]
	},
	{
		id: 'conv-8',
		title: 'Verjaardagscadeau ideeen',
		createdAt: daysAgo(7),
		updatedAt: daysAgo(7),
		mood: 'happy',
		duration: 300,
		tags: ['cadeau', 'verjaardag'],
		summary:
			'Brainstorm over verjaardagscadeau-ideeën voor een techliefhebber. Van Raspberry Pi projecten tot escape room ervaringen.',
		messages: [
			{
				id: 'msg-8-1',
				role: 'user',
				content:
					'Mijn vriend wordt volgende week 30 en hij is een echte techneut. Heb je cadeau-ideeën?',
				timestamp: daysAgo(7)
			},
			{
				id: 'msg-8-2',
				role: 'assistant',
				content:
					'Leuk! Voor een techliefhebber die 30 wordt, heb ik een paar toffe ideeën. In welke prijsklasse zit je, en heeft hij specifieke hobby\'s binnen tech? Denk aan programmeren, gadgets, domotica, gaming?',
				timestamp: daysAgo(7)
			},
			{
				id: 'msg-8-3',
				role: 'user',
				content:
					'Hij houdt van programmeren en tinkeren met elektronica. Budget rond de 75 euro.',
				timestamp: daysAgo(7)
			},
			{
				id: 'msg-8-4',
				role: 'assistant',
				content:
					'Perfect, dan heb ik een paar leuke suggesties! Een Raspberry Pi 5 starterkit met behuizing en SD-kaart is altijd een schot in de roos. Of een Arduino-sensorenpakket waarmee hij eigen projecten kan bouwen. Als hij van uitdagingen houdt: een tech-escape room ervaring. En als je iets persoonlijkers wilt: een gepersonaliseerde PCB-sleutelhanger met zijn naam erin geëtst. Tot slot: het boek "Crafting Interpreters" is een aanrader voor programmeurs die iets uitdagends willen leren.',
				timestamp: daysAgo(7)
			},
			{
				id: 'msg-8-5',
				role: 'user',
				content: 'Die Raspberry Pi kit klinkt perfect! Waar kan ik die het beste bestellen?',
				timestamp: daysAgo(7)
			},
			{
				id: 'msg-8-6',
				role: 'assistant',
				content:
					'In Nederland kun je het beste kijken bij SOS Solutions of Kiwi Electronics, dat zijn gespecialiseerde webshops die betrouwbaar zijn en snel leveren. Ze hebben complete startkits met alles erbij. Een andere goede optie is The Pi Hut, die verschepen ook naar Nederland. Bestel wel op tijd als het volgende week al is, dan ben je zeker van levering!',
				timestamp: daysAgo(7)
			}
		]
	},
	{
		id: 'conv-9',
		title: 'Energiebesparing tips',
		createdAt: daysAgo(9),
		updatedAt: daysAgo(9),
		mood: 'neutral',
		duration: 540,
		tags: ['energie', 'besparen'],
		summary:
			'Praktische tips voor energiebesparing in huis. Van slimme thermostaat tot LED-verlichting en isolatie, met geschatte besparingen.',
		messages: [
			{
				id: 'msg-9-1',
				role: 'user',
				content:
					'Mijn energierekening is flink gestegen. Heb je tips om energie te besparen in huis?',
				timestamp: daysAgo(9)
			},
			{
				id: 'msg-9-2',
				role: 'assistant',
				content:
					'Vervelend om te horen! Er zijn gelukkig veel manieren om te besparen. De grootste besparingen zitten in verwarming, die is vaak 60% van je energierekening. Heb je al een slimme thermostaat? En weet je hoe goed je huis geisoleerd is?',
				timestamp: daysAgo(9)
			},
			{
				id: 'msg-9-3',
				role: 'user',
				content:
					'Nog geen slimme thermostaat. Het is een jaren-70 huis, dus isolatie kan beter denk ik.',
				timestamp: daysAgo(9)
			},
			{
				id: 'msg-9-4',
				role: 'assistant',
				content:
					'Dan liggen er grote kansen! Hier mijn top vijf: Een slimme thermostaat zoals de Tado of Nest bespaart gemiddeld 15-20% op je stookkosten. Tochtstrips en radiatorfolie zijn goedkope ingrepen met direct effect. Overstappen naar LED-verlichting bespaart tot 80% op je lichtkosten. En voor de langere termijn: spouwmuurisolatie is vaak binnen een dag klaar en bespaart honderden euros per jaar.',
				timestamp: daysAgo(9)
			},
			{
				id: 'msg-9-5',
				role: 'user',
				content: 'Wat kost zo\'n slimme thermostaat en installeer ik die zelf?',
				timestamp: daysAgo(9)
			},
			{
				id: 'msg-9-6',
				role: 'assistant',
				content:
					'Een Tado starterkit kost rond de 200 euro en is prima zelf te installeren als je een beetje handig bent. Je vervangt gewoon je oude thermostaat en verbindt de Tado met je wifi. In de app stel je schema\'s in en hij leert zelfs van je gedrag. Het mooie is dat je per kamer kunt regelen met extra radiatorknoppen, dan verwarm je alleen de kamers die je gebruikt.',
				timestamp: daysAgo(9)
			},
			{
				id: 'msg-9-7',
				role: 'user',
				content: 'Klinkt goed, en zijn er subsidies voor isolatie?',
				timestamp: daysAgo(9)
			},
			{
				id: 'msg-9-8',
				role: 'assistant',
				content:
					'Ja, via de ISDE-regeling kun je subsidie krijgen voor isolatiemaatregelen. Voor spouwmuurisolatie is dat ongeveer 4 euro per vierkante meter, en voor vloerisolatie rond de 5 euro per vierkante meter. Je kunt dit combineren met de energiebespaarlening van het Nationaal Warmtefonds, met hele gunstige rente. Check ook je gemeente, veel gemeenten hebben extra regelingen bovenop de landelijke subsidies.',
				timestamp: daysAgo(9)
			}
		]
	},
	{
		id: 'conv-10',
		title: 'Mediteren voor beginners',
		createdAt: daysAgo(12),
		updatedAt: daysAgo(11),
		mood: 'playful',
		duration: 360,
		tags: ['gezondheid', 'meditatie'],
		summary:
			'Introductie tot meditatie voor beginners. Verschillende technieken besproken, van ademhalingsoefeningen tot bodyscan, met praktische tips om een routine op te bouwen.',
		messages: [
			{
				id: 'msg-10-1',
				role: 'user',
				content:
					'Ik wil beginnen met mediteren maar weet niet waar ik moet beginnen. Het lijkt me zo moeilijk om stil te zitten.',
				timestamp: daysAgo(12)
			},
			{
				id: 'msg-10-2',
				role: 'assistant',
				content:
					'Dat denken veel mensen, maar het goede nieuws is: mediteren hoeft helemaal niet moeilijk te zijn! Het gaat er niet om dat je aan niets denkt, maar dat je leert je gedachten te observeren zonder erin mee te gaan. Begin klein, zelfs 3 minuten per dag is al waardevol. Zal ik je een simpele techniek uitleggen?',
				timestamp: daysAgo(12)
			},
			{
				id: 'msg-10-3',
				role: 'user',
				content: 'Ja graag! Iets wat ik morgenochtend meteen kan proberen.',
				timestamp: daysAgo(12)
			},
			{
				id: 'msg-10-4',
				role: 'assistant',
				content:
					'Probeer dit: ga comfortabel zitten, sluit je ogen en focus op je ademhaling. Adem in door je neus, tel tot 4. Houd vast, tel tot 4. Adem uit door je mond, tel tot 6. Herhaal dit 5 keer. Als je gedachten afdwalen, en dat gaat zeker gebeuren, breng je aandacht gewoon vriendelijk terug naar je adem. Geen oordeel, geen frustratie. Dat terugbrengen van je aandacht IS de oefening!',
				timestamp: daysAgo(12)
			},
			{
				id: 'msg-10-5',
				role: 'user',
				content: 'Dat klinkt haalbaar! Zijn er ook apps die kunnen helpen?',
				timestamp: daysAgo(11)
			},
			{
				id: 'msg-10-6',
				role: 'assistant',
				content:
					'Zeker! Headspace en Calm zijn de bekendste en hebben geleide meditaties in het Nederlands. Als je iets gratis wilt: Insight Timer heeft duizenden gratis meditaties. Voor Nederlandstalig is de app Petit BamBou ook heel fijn. Begin met hun beginnerscursus van 10 sessies, dat bouwt mooi op. En weet je wat het allerbelangrijkste is? Maak er een vaste gewoonte van, bijvoorbeeld elke ochtend na het opstaan. Regelmaat is belangrijker dan duur!',
				timestamp: daysAgo(11)
			}
		]
	}
];
