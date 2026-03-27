<script lang="ts">
  import VoiceInterface from '$lib/components/voicelab/VoiceInterface.svelte';
  import VoiceControls from '$lib/components/voicelab/VoiceControls.svelte';
  import ConversationHistory from '$lib/components/voicelab/ConversationHistory.svelte';
  import { voiceStore } from '$lib/stores/voice.svelte';
</script>

<div class="flex min-h-[calc(100vh-6rem)] flex-col items-center {voiceStore.state !== 'idle' ? 'bg-primary/[0.02]' : ''} transition-colors duration-500">
  <!-- Voice section takes center of screen -->
  <div class="flex flex-1 flex-col items-center justify-center">
    <VoiceInterface />
    <VoiceControls />
  </div>

  <!-- Conversation history at the bottom -->
  <ConversationHistory />
</div>
