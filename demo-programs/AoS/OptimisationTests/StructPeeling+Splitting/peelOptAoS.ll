; ModuleID = 'peelOptAoS.bc'
source_filename = "unpeeledAoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeOneOld = type { i32, double, i32 }
%struct.nodeOneOldCold = type { double, i8, double, double, double, double, float }

@arrayOneOld = dso_local global [999999 x %struct.nodeOneOld] zeroinitializer, align 16
@arrayTwoOld = dso_local global [999999 x %struct.nodeOneOld] zeroinitializer, align 16
@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [8 x i8] c"%d\0A---\0A\00", align 1
@.str.2 = private unnamed_addr constant [8 x i8] c"%f\0A---\0A\00", align 1
@arrayOneOldCold = dso_local global [999999 x %struct.nodeOneOldCold] zeroinitializer, align 16

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOneOld(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32, ptr %i, align 4
  %idxprom = sext i32 %2 to i64
  %arrayidx = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  store i32 1, ptr %a, align 16
  %3 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %3 to i64
  %arrayidx2 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx2, i32 0, i32 1
  store double 1.000000e+01, ptr %b, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %4 to i64
  %arrayidx4 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx4, i32 0, i32 2
  store i32 9, ptr %c, align 16
  %5 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %5 to i64
  %arrayidx6 = getelementptr inbounds [999999 x %struct.nodeOneOldCold], ptr @arrayOneOldCold, i64 0, i64 %idxprom5
  %d = getelementptr inbounds %struct.nodeOneOldCold, ptr %arrayidx6, i32 0, i32 0
  store double 2.300000e+01, ptr %d, align 8
  %6 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %6 to i64
  %arrayidx8 = getelementptr inbounds [999999 x %struct.nodeOneOldCold], ptr @arrayOneOldCold, i64 0, i64 %idxprom7
  %e = getelementptr inbounds %struct.nodeOneOldCold, ptr %arrayidx8, i32 0, i32 1
  store i8 97, ptr %e, align 16
  %7 = load i32, ptr %i, align 4
  %idxprom9 = sext i32 %7 to i64
  %arrayidx10 = getelementptr inbounds [999999 x %struct.nodeOneOldCold], ptr @arrayOneOldCold, i64 0, i64 %idxprom9
  %f = getelementptr inbounds %struct.nodeOneOldCold, ptr %arrayidx10, i32 0, i32 2
  store double 2.300000e+01, ptr %f, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %8 to i64
  %arrayidx12 = getelementptr inbounds [999999 x %struct.nodeOneOldCold], ptr @arrayOneOldCold, i64 0, i64 %idxprom11
  %g = getelementptr inbounds %struct.nodeOneOldCold, ptr %arrayidx12, i32 0, i32 3
  store double 2.300000e+01, ptr %g, align 16
  %9 = load i32, ptr %i, align 4
  %idxprom13 = sext i32 %9 to i64
  %arrayidx14 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom13
  %a15 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx14, i32 0, i32 0
  store i32 1, ptr %a15, align 16
  %10 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %10 to i64
  %arrayidx17 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 1
  store double 1.000000e+01, ptr %b18, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom19 = sext i32 %11 to i64
  %arrayidx20 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom19
  %c21 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx20, i32 0, i32 2
  store i32 9, ptr %c21, align 16
  %12 = load i32, ptr %i, align 4
  %idxprom22 = sext i32 %12 to i64
  %arrayidx23 = getelementptr inbounds [999999 x %struct.nodeOneOldCold], ptr @arrayOneOldCold, i64 0, i64 %idxprom22
  %d24 = getelementptr inbounds %struct.nodeOneOldCold, ptr %arrayidx23, i32 0, i32 0
  store double 2.300000e+01, ptr %d24, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom25 = sext i32 %13 to i64
  %arrayidx26 = getelementptr inbounds [999999 x %struct.nodeOneOldCold], ptr @arrayOneOldCold, i64 0, i64 %idxprom25
  %e27 = getelementptr inbounds %struct.nodeOneOldCold, ptr %arrayidx26, i32 0, i32 1
  store i8 97, ptr %e27, align 16
  %14 = load i32, ptr %i, align 4
  %idxprom28 = sext i32 %14 to i64
  %arrayidx29 = getelementptr inbounds [999999 x %struct.nodeOneOldCold], ptr @arrayOneOldCold, i64 0, i64 %idxprom28
  %f30 = getelementptr inbounds %struct.nodeOneOldCold, ptr %arrayidx29, i32 0, i32 2
  store double 2.300000e+01, ptr %f30, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom31 = sext i32 %15 to i64
  %arrayidx32 = getelementptr inbounds [999999 x %struct.nodeOneOldCold], ptr @arrayOneOldCold, i64 0, i64 %idxprom31
  %g33 = getelementptr inbounds %struct.nodeOneOldCold, ptr %arrayidx32, i32 0, i32 3
  store double 2.300000e+01, ptr %g33, align 16
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %16 = load i32, ptr %i, align 4
  %inc = add nsw i32 %16, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeOneOld(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 30
  br i1 %cmp, label %for.body, label %for.end21

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 1
  %4 = load double, ptr %b, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %5 to i64
  %arrayidx5 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom4
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 0
  %6 = load i32, ptr %a, align 16
  %conv = sitofp i32 %6 to double
  %7 = call double @llvm.fmuladd.f64(double %4, double %conv, double 2.000000e+01)
  %conv6 = fptosi double %7 to i32
  %8 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %8 to i64
  %arrayidx8 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 0
  store i32 %conv6, ptr %a9, align 16
  %9 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %9 to i64
  %arrayidx11 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom10
  %a12 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 0
  %10 = load i32, ptr %a12, align 16
  %11 = load i32, ptr %i, align 4
  %idxprom13 = sext i32 %11 to i64
  %arrayidx14 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom13
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx14, i32 0, i32 2
  %12 = load i32, ptr %c, align 16
  %div = sdiv i32 %10, %12
  %sub = sub nsw i32 %div, 297
  %conv15 = sitofp i32 %sub to double
  %13 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %13 to i64
  %arrayidx17 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 1
  store double %conv15, ptr %b18, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %14 = load i32, ptr %i, align 4
  %inc = add nsw i32 %14, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !8

for.end:                                          ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %15 = load i32, ptr %j, align 4
  %inc20 = add nsw i32 %15, 1
  store i32 %inc20, ptr %j, align 4
  br label %for.cond, !llvm.loop !9

for.end21:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.fmuladd.f64(double, double, double) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multArrays(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 30
  br i1 %cmp, label %for.body, label %for.end21

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  %4 = load i32, ptr %a, align 16
  %5 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %5 to i64
  %arrayidx5 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom4
  %a6 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 0
  %6 = load i32, ptr %a6, align 16
  %mul = mul nsw i32 %4, %6
  %7 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %7 to i64
  %arrayidx8 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 0
  store i32 %mul, ptr %a9, align 16
  %8 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %8 to i64
  %arrayidx11 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom10
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 1
  %9 = load double, ptr %b, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %10 to i64
  %arrayidx13 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom12
  %b14 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx13, i32 0, i32 1
  %11 = load double, ptr %b14, align 8
  %mul15 = fmul double %9, %11
  %12 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %12 to i64
  %arrayidx17 = getelementptr inbounds [999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 1
  store double %mul15, ptr %b18, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %13 = load i32, ptr %i, align 4
  %inc = add nsw i32 %13, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !10

for.end:                                          ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %14 = load i32, ptr %j, align 4
  %inc20 = add nsw i32 %14, 1
  store i32 %inc20, ptr %j, align 4
  br label %for.cond, !llvm.loop !11

for.end21:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 999999, ptr %n, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %0, 100
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i32, ptr %n, align 4
  call void @populateNodeOneOld(i32 noundef %1)
  %2 = load i32, ptr %n, align 4
  call void @populateNodeOneOld(i32 noundef %2)
  %3 = load i32, ptr %n, align 4
  call void @multNodeOneOld(i32 noundef %3)
  %4 = load i32, ptr %n, align 4
  call void @multArrays(i32 noundef %4)
  %5 = load i32, ptr %i, align 4
  %cmp1 = icmp eq i32 %5, 77
  br i1 %cmp1, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  %call = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %6 = load i32, ptr getelementptr inbounds ([999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 5000), align 16
  %call2 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %6)
  %7 = load double, ptr getelementptr inbounds ([999999 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 5000, i32 1), align 8
  %call3 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %7)
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %8 = load i32, ptr %i, align 4
  %inc = add nsw i32 %8, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !12

for.end:                                          ; preds = %for.cond
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #2

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 893416051d517d979481510fe3b52d83ba0d1e01)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
