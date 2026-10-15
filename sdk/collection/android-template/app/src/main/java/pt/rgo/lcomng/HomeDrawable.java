package pt.rgo.lcomng;

/** Native library return, drawn in the same 24-unit stroke grid as SDK icons. */
final class HomeDrawable extends android.graphics.drawable.Drawable {
    private final android.graphics.Paint paint = new android.graphics.Paint(android.graphics.Paint.ANTI_ALIAS_FLAG);
    private int fill, ink;
    HomeDrawable(int fill,int ink) { this.fill=fill;this.ink=ink; }
    void colors(int fill,int ink) {
        if(this.fill==fill && this.ink==ink)return;
        this.fill=fill;this.ink=ink;invalidateSelf();
    }
    @Override public void draw(android.graphics.Canvas canvas) {
        android.graphics.Rect bounds=getBounds();
        paint.setStyle(android.graphics.Paint.Style.FILL);paint.setColor(fill);
        canvas.drawRect(bounds,paint);
        float size=Math.min(bounds.width(),bounds.height())*24f/44f;
        canvas.save();canvas.translate(bounds.exactCenterX()-size/2,bounds.exactCenterY()-size/2);canvas.scale(size/24,size/24);
        paint.setStyle(android.graphics.Paint.Style.STROKE);paint.setStrokeWidth(2);
        paint.setStrokeJoin(android.graphics.Paint.Join.ROUND);paint.setStrokeCap(android.graphics.Paint.Cap.ROUND);paint.setColor(ink);
        android.graphics.Path path=new android.graphics.Path();
        path.moveTo(3,11);path.lineTo(12,3);path.lineTo(21,11);
        path.moveTo(5,10);path.lineTo(5,21);path.lineTo(10,21);path.lineTo(10,15);path.lineTo(14,15);path.lineTo(14,21);path.lineTo(19,21);path.lineTo(19,10);
        canvas.drawPath(path,paint);canvas.restore();
    }
    @Override public void setAlpha(int alpha) { paint.setAlpha(alpha); }
    @Override public void setColorFilter(android.graphics.ColorFilter filter) { paint.setColorFilter(filter); }
    @Override public int getOpacity() { return android.graphics.PixelFormat.TRANSLUCENT; }
}
