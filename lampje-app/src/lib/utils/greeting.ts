export function getDutchGreeting(): string {
	const hour = new Date().getHours();
	if (hour < 6) return 'Goedenacht';
	if (hour < 12) return 'Goedemorgen';
	if (hour < 18) return 'Goedemiddag';
	return 'Goedenavond';
}

export function getDutchDate(): string {
	return new Date().toLocaleDateString('nl-NL', {
		weekday: 'long',
		day: 'numeric',
		month: 'long'
	});
}
