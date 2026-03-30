import { PostHog } from 'posthog-node';

const client = new PostHog('phc_3aUVIjKcHaatORIsyISwUjsGGEJdUyupj3MKGDFQrgy', {
	host: 'https://eu.i.posthog.com'
});

export function trackEvent(
	userId: string,
	event: string,
	properties?: Record<string, unknown>
) {
	client.capture({
		distinctId: userId,
		event,
		properties
	});
}
