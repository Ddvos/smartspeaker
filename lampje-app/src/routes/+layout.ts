import posthog from 'posthog-js';
import { browser } from '$app/environment';

export const load = async ({ data }) => {
	if (browser) {
		posthog.init('phc_3aUVIjKcHaatORIsyISwUjsGGEJdUyupj3MKGDFQrgy', {
			api_host: 'https://eu.i.posthog.com'
		});
	}

	return data;
};
