<script lang="ts">
  import { voiceStore } from '$lib/stores/voice.svelte';

  interface DemoMessage {
    role: 'user' | 'assistant';
    content: string;
    time: string;
  }

  const demoMessages: DemoMessage[] = [
    {
      role: 'user',
      content: 'Hoe wordt het weer vandaag?',
      time: '14:32'
    },
    {
      role: 'assistant',
      content: 'Het wordt vandaag 18 graden en overwegend bewolkt in Amsterdam. Vanmiddag is er kans op een bui, maar vanavond klaart het op.',
      time: '14:32'
    },
    {
      role: 'user',
      content: 'Moet ik een jas meenemen?',
      time: '14:33'
    },
    {
      role: 'assistant',
      content: 'Ja, ik zou een lichte regenjas aanraden. De temperatuur is prima, maar die bui in de middag kan je verrassen als je buiten bent.',
      time: '14:33'
    }
  ];
</script>

<div class="mt-12 w-full max-w-lg space-y-4">
  {#each demoMessages as msg}
    {#if msg.role === 'user'}
      <div class="ml-auto max-w-[80%]">
        <div class="rounded-xl rounded-br-md bg-surface-2 px-4 py-3 text-sm text-on-bg">
          {msg.content}
        </div>
        <p class="mt-1 text-right font-label text-[11px] text-on-bg-muted">{msg.time}</p>
      </div>
    {:else}
      <div class="mr-auto max-w-[80%]">
        <div class="rounded-xl rounded-bl-md bg-surface-1 px-4 py-3 text-sm text-on-bg">
          {msg.content}
        </div>
        <p class="mt-1 font-label text-[11px] text-on-bg-muted">{msg.time}</p>
      </div>
    {/if}
  {/each}

  {#if voiceStore.transcript}
    <div class="ml-auto max-w-[80%]">
      <div class="rounded-xl rounded-br-md bg-surface-2 px-4 py-3 text-sm text-on-bg">
        {voiceStore.transcript}
      </div>
      <p class="mt-1 text-right font-label text-[11px] text-on-bg-muted">zojuist</p>
    </div>
  {/if}

  {#if voiceStore.response}
    <div class="mr-auto max-w-[80%]">
      <div class="rounded-xl rounded-bl-md bg-surface-1 px-4 py-3 text-sm text-on-bg">
        {voiceStore.response}
      </div>
      <p class="mt-1 font-label text-[11px] text-on-bg-muted">zojuist</p>
    </div>
  {/if}
</div>
