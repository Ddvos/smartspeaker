import posthog from 'posthog-js';
import { browser } from '$app/environment';

export const load = async ({ data }) => {
	if (browser) {
		posthog.init('phc_3aUVIjKcHaatORIsyISwUjsGGEJdUyupj3MKGDFQrgy', {
			api_host: 'https://eu.i.posthog.com',
			capture_pageview: false,
			capture_pageleave: true,
			capture_exceptions: true
		});

		if (data.session?.user) {
			posthog.identify(data.session.user.id, {
				email: data.session.user.email,
				name: data.session.user.name
			});
		} else {
			posthog.reset();
		}
	}

	return data;
};
