declare module "litecam" {
    export class NodeCam {
        constructor();

        open(index: number): boolean;
        listMediaTypes(): any[];
        release(): void;
        setResolution(width: number, height: number): boolean;
        captureFrame(): Frame | null;
        getWidth(): number;
        getHeight(): number;

        createWindow(width: number, height: number, title: string): void;
        waitKey(key: string): boolean;
        showFrame(width: number, height: number, rgbdata: Buffer): void;
        drawContour(points: [number, number][]): void;
        drawText(text: string, x: number, y: number, fontSize: number, color: [number, number, number]): void;
    }

    export function getDeviceList(): string[];

    export function saveJpeg(filename: string, width: number, height: number, byteArray: Buffer): void;

    export interface Frame {
        width: number;
        height: number;
        size: number;
        data: Buffer;
    }
}
