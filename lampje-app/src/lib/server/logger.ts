import { NodeSDK } from '@opentelemetry/sdk-node';
import { OTLPLogExporter } from '@opentelemetry/exporter-logs-otlp-http';
import { BatchLogRecordProcessor } from '@opentelemetry/sdk-logs';
import { resourceFromAttributes } from '@opentelemetry/resources';
import { logs, SeverityNumber } from '@opentelemetry/api-logs';

const sdk = new NodeSDK({
	resource: resourceFromAttributes({
		'service.name': 'lampje-app'
	}),
	logRecordProcessor: new BatchLogRecordProcessor(
		new OTLPLogExporter({
			url: 'https://eu.i.posthog.com/i/v1/logs',
			headers: {
				Authorization: 'Bearer phc_3aUVIjKcHaatORIsyISwUjsGGEJdUyupj3MKGDFQrgy'
			}
		})
	)
});

sdk.start();

const otelLogger = logs.getLogger('lampje-app');

const severityMap = {
	debug: SeverityNumber.DEBUG,
	info: SeverityNumber.INFO,
	warn: SeverityNumber.WARN,
	error: SeverityNumber.ERROR
} as const;

type LogLevel = keyof typeof severityMap;

export function log(
	level: LogLevel,
	message: string,
	attributes?: Record<string, string | number | boolean>
) {
	otelLogger.emit({
		severityText: level,
		severityNumber: severityMap[level],
		body: message,
		attributes
	});
}
