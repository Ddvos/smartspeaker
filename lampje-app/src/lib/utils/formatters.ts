export function formatRelativeTime(date: Date): string {
	const now = new Date();
	const diff = now.getTime() - date.getTime();
	const minutes = Math.floor(diff / 60000);
	const hours = Math.floor(diff / 3600000);
	const days = Math.floor(diff / 86400000);

	if (minutes < 1) return 'zojuist';
	if (minutes < 60) return `${minutes} min geleden`;
	if (hours < 24) return `${hours} uur geleden`;
	if (days === 1) return 'gisteren';
	if (days < 7) return `${days} dagen geleden`;

	return date.toLocaleDateString('nl-NL', { day: 'numeric', month: 'short' });
}

export function formatDuration(seconds: number): string {
	const mins = Math.floor(seconds / 60);
	if (mins < 60) return `${mins} min`;
	const hours = Math.floor(mins / 60);
	const remainMins = mins % 60;
	return `${hours}u ${remainMins}m`;
}

export function formatDate(date: Date): string {
	return date.toLocaleDateString('nl-NL', { day: 'numeric', month: 'short' });
}

export function formatDateFull(date: Date): string {
	return date.toLocaleDateString('nl-NL', {
		weekday: 'long',
		day: 'numeric',
		month: 'long',
		year: 'numeric'
	});
}
