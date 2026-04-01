import posthog from 'posthog-js';
import { browser } from '$app/environment';

let micHoverVariant = $state<string | null>(null);

if (browser) {
	posthog.onFeatureFlags(() => {
		micHoverVariant = posthog.getFeatureFlag('mic-hover-color') as string | null;
	});
}

export const experimentStore = {
	get micHoverVariant() {
		return micHoverVariant;
	}
};
