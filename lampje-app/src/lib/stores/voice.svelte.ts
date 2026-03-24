import type { OrbState } from '$lib/types';

let orbState = $state<OrbState>('idle');
let transcript = $state('');
let response = $state('');

export const voiceStore = {
	get state() {
		return orbState;
	},
	get transcript() {
		return transcript;
	},
	get response() {
		return response;
	},

	startListening() {
		orbState = 'listening';
		transcript = '';
		response = '';
	},

	startThinking() {
		orbState = 'thinking';
	},

	startSpeaking(text: string) {
		orbState = 'speaking';
		response = text;
	},

	stop() {
		orbState = 'idle';
	},

	reset() {
		orbState = 'idle';
		transcript = '';
		response = '';
	},

	// Simulate a full voice cycle for demo purposes
	simulateCycle() {
		this.startListening();
		setTimeout(() => {
			transcript = 'Wat is het weer vandaag?';
			this.startThinking();
			setTimeout(() => {
				this.startSpeaking(
					'Het is vandaag 18 graden en bewolkt in Amsterdam. Vanmiddag wordt het iets warmer, rond de 20 graden.'
				);
				setTimeout(() => {
					this.stop();
				}, 3000);
			}, 1500);
		}, 2000);
	}
};
